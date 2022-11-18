#include "user.h"

User::User()
{
}

void User::setName(QString name) {
    this->name=name;
}

void User::setPassword(QString password){
    this->password=password;
}

QString User::getName(){
    return name;
}
