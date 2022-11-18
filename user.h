#ifndef USER_H
#define USER_H
#include <QString>


class User
{
public:
    User();
    void setName(QString name);
    void setPassword(QString password);
    QString getName();

private:
    QString name;
    QString password;
};

#endif // USER_H
