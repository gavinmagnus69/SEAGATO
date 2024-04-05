#include "User.h"
#include <string>

Entity::User::User() noexcept : yandex_token(""), tg_chat_id(""),
 user_id(0), user_password(""){}

Entity::User::User(const std::string& tkn, const std::string& chat_id,
             int usr_id, const std::string& pass) noexcept : 
             yandex_token(tkn), tg_chat_id(chat_id), user_id(usr_id), user_password(pass){}


//getters implementations
std::string Entity::User::get_yandex_token() const { return this->yandex_token;}
std::string Entity::User::get_tg_chat_id() const { return this->tg_chat_id; }
int Entity::User::get_user_id() const{ return this->user_id; }
std::string Entity::User::get_user_password() const { return this->get_user_password(); }

//setters implementations
void Entity::User::set_yandex_token(const std::string& tkn)
{
    this->yandex_token = tkn;
}

void Entity::User::set_tg_chat_id(const std::string& chat_id)
{
    this->tg_chat_id = chat_id;
}

void Entity::User::set_user_id(int id)
{
    this->user_id = id;
}

void Entity::User::set_user_password(const std::string& pass)
{
    this->user_password = pass;
}

