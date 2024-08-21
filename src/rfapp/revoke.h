#pragma once

#ifndef REVOKE_H
#define REVOKE_H

#include <string>
#include <string_view>
#include <vector>

namespace rfapp
{
class ActionBase
{
public:
    virtual void execute(void *arg_copy, size_t arg_size) = 0;
    virtual ~ActionBase() = default;
};

class Revoke
{
    Revoke() = default;
    ~Revoke();
    Revoke(const Revoke &) = delete;
    Revoke(Revoke &&) = delete;
    Revoke &operator=(const Revoke &) = delete;
    Revoke &operator=(Revoke &&) = delete;

    void setVarValue(void *var, const void *value, size_t size);
    void setStringValue(std::string *var, const std::string &value);
    void setAction(ActionBase *action, void *arg_copy, size_t arg_size);
    void revokeRecords();

    int startTransaction();
    void markTransaction(int id);
    void commitTransaction(int id);
    void backTransaction(int id);
    void rollbackTransaction(int id);
    void cancelTransaction();
    void completeTransaction();

    bool isTransacting() const;
    bool hasUncommittedTransaction() const;

private:
    enum class Type
    {
        Modify = 0,
        ModifyString,
        ModifyAction,
        Transaction,
        Mark,
        Commit
    };
    struct Record
    {
        Type type;
        void *temp = nullptr;
        void *copy = nullptr;
        size_t size = 0;
    };

    bool isModifyRecord(const Record &record) const;

    void clearRevoke();
    void revertValue(const Record &record);
    void revertVarValue(const Record &record);
    void revertString(const Record &record);
    void revertAction(const Record &record);
    void revertTransaction(size_t id, bool revert_group);

    std::vector<Record> records_;
    int transaction_ = 0;
};
} // namespace rfapp
#endif
