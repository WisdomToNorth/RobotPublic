#include "revoke.h"

#include <algorithm>
#include <cstring>

namespace rfapp
{
Revoke::~Revoke()
{
    clearRevoke();
}

void Revoke::setVarValue(void *var, const void *value, size_t size)
{
    if (var && value && size)
    {
        Record record;
        record.type = Type::Modify;

        record.size = size;
        if (size > sizeof(record.temp))
        {
            record.copy = malloc(size);
            if (record.copy == NULL)
            {
                return;
            }
            else
            {
                memcpy(record.copy, var, size);
            }
        }
        else
        {
            memcpy(&record.copy, var, size);
        }
        record.temp = var;
        memcpy(var, value, size);
        records_.push_back(record);
    }
}
void Revoke::setStringValue(std::string *var, const std::string &value)
{
    if (var)
    {
        Record record;
        record.type = Type::ModifyString;

        record.size = sizeof(std::string);
        if (record.size < sizeof(record.copy))
        {
            memcpy(&(record.copy), var->c_str(), record.size + 1);
        }
        else
        {
            record.copy = malloc(record.size + 1);
            if (record.copy == NULL)
            {
                // handle error
            }
            else
            {
                memcpy(record.copy, var->c_str(), record.size + 1);
            }
        }

        record.temp = var;
        var->assign(value);
        records_.push_back(record);
    }
}
void Revoke::setAction(ActionBase *action, void *arg_copy, size_t arg_size)
{
    if (!action)
    {
        return;
    }
    Record record;
    record.type = Type::ModifyAction;
    record.temp = action;
    record.size = arg_size;
    record.copy = arg_copy;
    records_.push_back(record);
}

int Revoke::startTransaction()
{
    ++transaction_;
    Record record;
    record.type = Type::Transaction;
    record.size = transaction_;
    records_.push_back(record);
    return transaction_;
}
void Revoke::markTransaction(int id)
{
    if (id)
    {
        Record record;
        record.type = Type::Mark;
        record.size = id;
        records_.push_back(record);
    }
}
void Revoke::commitTransaction(int id)
{
    if (id)
    {
        Record record;
        record.type = Type::Commit;
        record.size = id;
        records_.push_back(record);
    }
}
void Revoke::backTransaction(int id)
{
    revertTransaction(id, true);
}
void Revoke::rollbackTransaction(int id)
{
    revertTransaction(id, false);
}

void Revoke::cancelTransaction()
{
    revertTransaction(1, false);
}

void Revoke::completeTransaction()
{
    clearRevoke();
    transaction_ = 0;
}

bool Revoke::isTransacting() const
{
    return (transaction_ > 0);
}

void Revoke::revokeRecords()
{
    for (auto it = records_.rbegin(), end = records_.rend(); it != end; ++it)
    {
        revertValue(*it);
    }
    if (!records_.empty())
    {
        records_.clear();
    }
    transaction_ = 0;
}

bool Revoke::hasUncommittedTransaction() const
{
    if (!isTransacting())
    {
        return false;
    }
    bool has_modify = false;
    for (auto it = records_.rbegin(); it != records_.rend(); ++it)
    {
        if (it->type == Type::Commit)
        {
            auto trans_begin_it = std::find_if(
                it, records_.rend(),
                [trans_id = it->size](const Record &record)
                { return (record.type == Type::Transaction && record.size == trans_id); });

            if (trans_begin_it != records_.rend())
            {
                it = trans_begin_it;
            }
            continue;
        }
        if (isModifyRecord(*it))
        {
            has_modify = true;
            continue;
        }
        if (it->type == Type::Transaction && has_modify)
        {
            return true;
        }
    }
    return false;
}

bool Revoke::isModifyRecord(const Record &record) const
{
    return (record.type == Type::Modify || record.type == Type::ModifyString
            || record.type == Type::ModifyAction);
}

void Revoke::clearRevoke()
{
    for (const auto &record : records_)
    {
        if (record.temp && record.copy)
        {
            if (record.type == Type::Modify)
            {
                if (record.size > sizeof(record.copy))
                {
                    free(record.copy);
                }
            }
            else if (record.type == Type::ModifyString)
            {
                if (record.size >= sizeof(record.copy))
                {
                    free(record.copy);
                }
            }
        }
    }
    records_.clear();
}

void Revoke::revertValue(const Record &record)
{
    if (record.type == Type::Modify)
    {
        revertVarValue(record);
    }
    else if (record.type == Type::ModifyString)
    {
        revertString(record);
    }
    else if (record.type == Type::ModifyAction)
    {
        revertAction(record);
    }
}

void Revoke::revertVarValue(const Record &record)
{
    if (record.temp && record.size > 0)
    {
        if (record.size > sizeof(record.copy))
        {
            memcpy(record.temp, record.copy, record.size);
            free(record.copy);
        }
        else
        {
            memcpy(record.temp, &record.copy, record.size);
        }
    }
}

void Revoke::revertString(const Record &record)
{
    if (record.temp)
    {
        std::string *var = static_cast<std::string *>(record.temp);
        if (record.size < sizeof(record.copy))
        {
            var->assign(static_cast<char *>(record.copy));
        }
        else
        {
            if (record.copy)
            {
                var->assign(static_cast<char *>(record.copy));
                free(record.copy);
            }
        }
    }
}

void Revoke::revertAction(const Record &record)
{
    if (record.type != Type::ModifyAction || !record.temp)
    {
        return;
    }
    ActionBase *action = static_cast<ActionBase *>(record.temp);
    action->execute(record.copy, record.size);
    if (record.copy)
    {
        free(record.copy);
    }
}

void Revoke::revertTransaction(size_t id, bool revert_step)
{
    if (id)
    {
        auto rbegin = records_.rbegin();
        auto rend = records_.rend();
        auto result =
            std::find_if(rbegin, rend,
                         [id, revert_step](const Record &record)
                         {
                             if ((record.type == Type::Mark && revert_step)
                                 || record.type == Type::Commit || record.type == Type::Transaction)
                             {
                                 return (record.size == id) ? true : false;
                             }
                             else
                             {
                                 return false;
                             }
                         });
        if (result != rend)
        {
            if (result->type == Type::Transaction || result->type == Type::Commit)
            {
                if (result == rbegin)
                {
                    return;
                }
                else
                {
                    --result;
                }
            }
            for (auto it = rbegin; it <= result; ++it)
            {
                revertValue(*it);
            }
            auto dist = std::distance(result, rend);
            records_.erase(records_.cbegin() + dist - 1, records_.cend());
        }
    }
}
} // namespace rfapp
