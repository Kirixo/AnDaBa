#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H

#include <QString>
#include <mutex>


class AccountInfo
{
private:
    static AccountInfo * pinstance_;
    static std::mutex mutex_;

protected:
    AccountInfo(uint user_id, const QString login, const QString email, const QString avatar = NULL):
        _user_id(user_id), _login(login), _email(email), _avatar(avatar)
    {
    }
    ~AccountInfo() {}
    uint _user_id;
    QString _login;
    QString _email;
    QString _avatar;

public:
    AccountInfo(AccountInfo &other) = delete;
    void operator=(const AccountInfo &) = delete;

    static AccountInfo *GetInstance();
    static AccountInfo *GetInstance(uint user_id, const QString login, const QString email, const QString avatar);
    /*
    void SomeBusinessLogic()
    {
    }*/

    QString login() const{
        return _login;
    }
    int user_id() const{
        return _user_id;
    }
    QString email() const{
        return _email;
    }
    QString avatar() const{
        return _avatar;
    }
};

#endif // ACCOUNTINFO_H
