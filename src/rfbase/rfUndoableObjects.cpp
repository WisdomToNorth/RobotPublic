#include "rfUndoableObjects.h"

#include <algorithm>
#include <cassert>

namespace rfbase
{
std::unordered_map<std::shared_ptr<Storable>, std::shared_ptr<UndoableObjectNode>>::iterator
UndoableObjects::start_read_object()
{
    return objects_.begin();
}

std::shared_ptr<Storable> UndoableObjects::read_object(
    std::unordered_map<std::shared_ptr<Storable>, std::shared_ptr<UndoableObjectNode>>::iterator
        &p_it)
{
    while (p_it != objects_.end())
    {
        std::shared_ptr<UndoableObjectNode> curr_node = p_it->second;
        // skip objects getting alive only by redo
        if (curr_node != nullptr && curr_node->level <= this->stack_level_)
        {
            return curr_node->cur_data_;
        }
        ++p_it;
    }
    return nullptr;
}

void UndoableObjects::insert(std::shared_ptr<Storable> p_object)
{
    std::lock_guard<std::mutex> lock(mutex_);
    disable_redo();
    std::shared_ptr<UndoableObjectNode> node =
        std::make_shared<UndoableObjectNode>(p_object, stack_level_);
    objects_[p_object] = node;
}

bool UndoableObjects::delete_object(std::shared_ptr<Storable> p_object)
{
    // search p_object in the map
    auto object_node_iter = objects_.find(p_object);
    if (object_node_iter == objects_.end())
    {
        return false;
    }

    disable_redo();

    std::list<std::shared_ptr<UndoableObjectNode>> *curr_delete_list =
        deleted_objects_stack_.empty() ? nullptr : &deleted_objects_stack_.back();

    if (curr_delete_list != nullptr)
    {
        std::shared_ptr<UndoableObjectNode> object_node = object_node_iter->second;

        if (object_node->level < this->stack_level_) //??
        {
            // add curr_ob to the current delete list to make Undo possible.
            curr_delete_list->push_back(object_node);
        }
        else if (object_node->undo_object_ != nullptr)
        {
            // add curr_ob.undo_object_ to the current delete list to make Undo possible.
            curr_delete_list->push_back(object_node->undo_object_);
        }
    }

    objects_.erase(p_object);
    return true;
}

void UndoableObjects::generate_snapshot()
{
    std::lock_guard<std::mutex> lock(mutex_);
    disable_redo();
    std::list<std::shared_ptr<UndoableObjectNode>> new_delete_list;
    deleted_objects_stack_.push_back(new_delete_list);
    ++stack_level_;
}

bool UndoableObjects::undo(std::list<std::shared_ptr<Storable>> &p_cancelled_objects,
                           std::list<std::shared_ptr<Storable>> &p_restored_objects)
{
    if (stack_level_ == 0)
    {
        return false; // no more undo possible
    }

    std::unordered_map<std::shared_ptr<Storable>, std::shared_ptr<UndoableObjectNode>>
        new_added_objects;
    std::vector<std::shared_ptr<Storable>> keys_to_erase;

    for (auto it = objects_.begin(); it != objects_.end(); ++it)
    {
        auto curr_data = it->first;
        auto curr_node = it->second;
        if ((curr_node->level == stack_level_) && (curr_node->undo_object_ != nullptr))
        {
            // replace the current object by its previous state.
            auto &undo_object = curr_node->undo_object_;
            p_restored_objects.push_back(undo_object->cur_data_);
            p_cancelled_objects.push_back(curr_node->cur_data_);

            undo_object->redo_object_ = curr_node;
            keys_to_erase.push_back(curr_data);
            new_added_objects[undo_object->cur_data_] = undo_object;
        }
    }

    for (auto key : keys_to_erase)
    {
        objects_.erase(key);
    }

    objects_.insert(new_added_objects.begin(), new_added_objects.end());

    std::list<std::shared_ptr<UndoableObjectNode>> &curr_delete_list =
        deleted_objects_stack_.at(stack_level_ - 1);
    for (auto &curr_deleted_node : curr_delete_list)
    {
        if (objects_.find(curr_deleted_node->cur_data_) == objects_.end())
        {
            objects_[curr_deleted_node->cur_data_] = curr_deleted_node;
            p_restored_objects.push_back(curr_deleted_node->cur_data_);
        }
    }

    --this->stack_level_;
    redo_possible_ = true;
    return true;
}

bool UndoableObjects::redo(std::list<std::shared_ptr<Storable>> &p_cancelled_objects,
                           std::list<std::shared_ptr<Storable>> &p_restored_objects)
{
    if (this->stack_level_ >= int(deleted_objects_stack_.size()))
    {
        return false; // already at the top level
    }

    for (auto &[curr_data, curr_node] : objects_)
    {
        if ((curr_node->level == this->stack_level_) && (curr_node->redo_object_ != nullptr))
        {
            // Object was created on a higher level and changed on the current level,
            // replace the higher level object by the object on the current layer.
            curr_node->redo_object_->undo_object_ = curr_node;

            p_restored_objects.push_back(curr_node->redo_object_->cur_data_);
            p_cancelled_objects.push_back(curr_node->cur_data_);

            objects_[curr_data] = curr_node->redo_object_;
        }
        else if (curr_node->level == this->stack_level_ + 1)
        {
            // Object was created on the next level, allow it to be restored.
            assert(curr_node->cur_data_ == curr_data);
            p_restored_objects.push_back(curr_node->cur_data_);
        }
    }

    // Delete the objects, which were deleted on the current level, again.
    auto &curr_delete_list = deleted_objects_stack_.at(stack_level_);
    for (auto &curr_deleted_node : curr_delete_list)
    {
        while (curr_deleted_node->redo_object_ != nullptr &&
               curr_deleted_node->redo_object_->level <= this->stack_level_)
        {
            curr_deleted_node = curr_deleted_node->redo_object_;
        }

        [[maybe_unused]] size_t erase_cnt = objects_.erase(curr_deleted_node->cur_data_);
        assert(erase_cnt == 1);

        if (std::find(p_restored_objects.begin(), p_restored_objects.end(),
                      curr_deleted_node->cur_data_) == p_restored_objects.end())
        {
            p_cancelled_objects.push_back(curr_deleted_node->cur_data_);
        }
    }

    ++this->stack_level_;
    return true;
}

bool UndoableObjects::pop_snapshot()
{
    disable_redo();
    if (stack_level_ == 0)
    {
        return false;
    }
    for (auto &curr_pair : objects_)
    {
        auto curr_node = curr_pair.second;
        if (curr_node->level == stack_level_ - 1)
        {
            if (curr_node->redo_object_ != nullptr &&
                curr_node->redo_object_->level == stack_level_)
            {
                curr_node->redo_object_->undo_object_ = curr_node->undo_object_;
                if (curr_node->undo_object_ != nullptr)
                {
                    curr_node->undo_object_->redo_object_ = curr_node->redo_object_;
                }
            }
        }
        else if (curr_node->level >= stack_level_)
        {
            --curr_node->level;
        }
    }
    size_t deleted_objects_stack_size = deleted_objects_stack_.size();
    if (deleted_objects_stack_size >= 2)
    {
        // join the top delete list with the delete list of the second top level
        auto &from_delete_list = deleted_objects_stack_.at(deleted_objects_stack_size - 1);
        auto &to_delete_list = deleted_objects_stack_.at(deleted_objects_stack_size - 2);
        for (auto &curr_deleted_node : from_delete_list)
        {
            if (curr_deleted_node->level < this->stack_level_ - 1)
            {
                to_delete_list.push_back(curr_deleted_node);
            }
            else if (curr_deleted_node->undo_object_ != nullptr)
            {
                to_delete_list.push_back(curr_deleted_node->undo_object_);
            }
        }
    }
    deleted_objects_stack_.pop_back();
    --stack_level_;
    return true;
}

void UndoableObjects::save_for_undo(std::shared_ptr<Storable> p_object)
{
    std::lock_guard<std::mutex> lock(mutex_);
    // search p_object in the map
    auto it = objects_.find(p_object);
    if (it == objects_.end())
    {
        // FRLog.warn("UndoableObjects.save_for_undo: object node not found");
        return;
    }

    disable_redo();
    auto curr_node = it->second;
    if (curr_node->level < this->stack_level_)
    {
        auto old_node = std::make_shared<UndoableObjectNode>(p_object->clone(), curr_node->level);
        old_node->undo_object_ = curr_node->undo_object_;
        old_node->redo_object_ = curr_node;
        curr_node->undo_object_ = old_node;
        curr_node->level = this->stack_level_;
        curr_node->cur_data_ = it->first;
    }
}

void UndoableObjects::disable_redo()
{
    if (!redo_possible_)
    {
        return;
    }
    redo_possible_ = false;
    // shorten the size of the deleted_objects_stack to this.stack_level
    deleted_objects_stack_.erase(deleted_objects_stack_.begin() + this->stack_level_,
                                 deleted_objects_stack_.end());
    for (auto it = objects_.begin(); it != objects_.end(); /* no increment here */)
    {
        auto curr_node = it->second;
        if (curr_node->level > this->stack_level_)
        {
            it = objects_.erase(it);
        }
        else
        {
            if (curr_node->level == this->stack_level_)
            {
                curr_node->redo_object_ = nullptr;
            }
            ++it;
        }
    }
}

int UndoableObjects::get_stack_level()
{
    return stack_level_;
}

int UndoableObjects::get_object_level(std::shared_ptr<Storable> p_object)
{
    auto it = objects_.find(p_object);
    if (it == objects_.end())
    {
        return -1;
    }
    return it->second->level;
}

size_t UndoableObjects::get_object_count()
{
    return objects_.size();
}
} // namespace rfbase
