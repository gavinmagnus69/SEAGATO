#ifndef USER_H
#define USER_H


#include <string>
namespace Entity
{
    class User
    {
        private:
            std::string yandex_token;
            std::string tg_chat_id;
            int user_id;
            std::string user_password;
        public:
            User() noexcept;
            User(const std::string& tkn, const std::string& chat_id,
             int usr_id, const std::string& pass) noexcept;
        public:
            //getters
            std::string get_yandex_token() const;
            std::string get_tg_chat_id() const;
            int get_user_id() const;
            std::string get_user_password() const;
        public:
            //setters
            void set_yandex_token(const std::string& token);
            void set_tg_chat_id(const std::string& chat_id);
            void set_user_id(int id);
            void set_user_password(const std::string& pass);
    };

}
#endif