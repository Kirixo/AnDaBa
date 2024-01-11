#include "accountinfo.h"

AccountInfo* AccountInfo::pinstance_{nullptr};
std::mutex AccountInfo::mutex_;

AccountInfo *AccountInfo::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new AccountInfo(NULL, NULL, NULL, NULL);
    }
    return pinstance_;
}

AccountInfo *AccountInfo::GetInstance(uint user_id, const QString login, const QString email, const QString avatar = NULL)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new AccountInfo(user_id, login, email, avatar);
    }
    return pinstance_;
}
