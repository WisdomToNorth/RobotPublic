#pragma once
#ifndef RF_UNDOABLEOBJECTS_H
#define RF_UNDOABLEOBJECTS_H

#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace rfbase
{
class Storable
{
public:
    virtual std::shared_ptr<Storable> clone() const = 0;
};

class UndoableObjectNode
{
public:
    std::shared_ptr<Storable> cur_data_;              // the object in the node
    std::shared_ptr<UndoableObjectNode> undo_object_; // the object to restore in an undo or null.
    std::shared_ptr<UndoableObjectNode> redo_object_; // the object to restore in a redo or null.
    int level; // the level in the Undo stack, where this node was inserted

    // Creates a new instance of UndoableObjectNode
    UndoableObjectNode(std::shared_ptr<Storable> p_object, int p_level)
        : cur_data_(p_object)
        , undo_object_(nullptr)
        , redo_object_(nullptr)
        , level(p_level)
    {
    }
};

class UndoableObjects
{
private:
    /**
     * The entries of this map are of type UndoableObject, the keys of type
     * UndoableObjects Storable.
     */
    std::unordered_map<std::shared_ptr<Storable>, std::shared_ptr<UndoableObjectNode>> objects_;
    /**
     * the lists of deleted objects on each undo level, which where already existing
     * before the previous snapshot.
     */
    std::vector<std::list<std::shared_ptr<UndoableObjectNode>>> deleted_objects_stack_;
    /** the current undo level */
    int stack_level_;
    bool redo_possible_;

private:
    std::mutex mutex_;

public:
    UndoableObjects()
        : objects_()
        , deleted_objects_stack_()
        , stack_level_(0)
        , redo_possible_(false)
    {
    }

    // Returns an iterator for sequential reading of the object list. Use it together with
    // read_object().
    std::unordered_map<std::shared_ptr<Storable>, std::shared_ptr<UndoableObjectNode>>::iterator
    start_read_object();

    // Reads the next object in this list. Returns null, if the list is exhausted. p_it must be
    // created by start_read_object.
    std::shared_ptr<Storable> read_object(
        std::unordered_map<std::shared_ptr<Storable>, std::shared_ptr<UndoableObjectNode>>::iterator
            &p_it);

    /** Adds p_object to the UndoableObjectsList. */
    void insert(std::shared_ptr<Storable> p_object);

    /**
     * Removes p_object from the top level of the UndoableObjectsList. Returns
     * false, if p_object was not found in the list.
     */
    bool delete_object(std::shared_ptr<Storable> p_object);

    /** Makes the current state of the list restorable by Undo. */
    void generate_snapshot();

    /**
     * Restores the situation before the last snapshot. Outputs the cancelled and
     * the restored objects (if != null) to enable the calling function to take additional actions
     * needed for these objects. Returns false, if no more undo is possible
     */
    bool undo(std::list<std::shared_ptr<Storable>> &p_cancelled_objects,
              std::list<std::shared_ptr<Storable>> &p_restored_objects);

    /**
     * Restores the situation before the last undo. Outputs the cancelled and the
     * restored objects (if != null) to enable the calling function to take additional actions
     * needed for these objects. Returns false, if no more redo is possible.
     */
    bool redo(std::list<std::shared_ptr<Storable>> &p_cancelled_objects,
              std::list<std::shared_ptr<Storable>> &p_restored_objects);

    /**
     * Removes the top snapshot from the undo stack, so that its situation cannot be
     * restored anymore. Returns false, if no more snapshot could be popped.
     */
    bool pop_snapshot();

    /**
     * Must be called before p_object will be modified after a snapshot for the
     * first time, if it may have existed before that snapshot.
     */
    void save_for_undo(std::shared_ptr<Storable> p_object);

    /** Must be called, if objects are changed for the first time after undo. */
    void disable_redo();

    // Easy for testing
    int get_stack_level();
    int get_object_level(std::shared_ptr<Storable> p_object);
    size_t get_object_count();
};
} // namespace rfbase
#endif
