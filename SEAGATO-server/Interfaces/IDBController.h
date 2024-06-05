#ifndef IDBCONTROLLER_H
#define IDBCONTROLLER_H

#include <string>
#include <variant>
#include <vector>


namespace Interface {


    class IBDController{
        public:
            //login token chat_id?
            virtual int add(const std::vector<std::string>& data) = 0;
            //get token and chat_id by login if registred
            virtual std::variant<std::string, int> find(const std::string& login) = 0;
            virtual int update(const std::vector<std::string>& data) = 0;
            virtual std::string get_token(const std::string& login) = 0;
            virtual std::string get_chatid(const std::string& login) = 0;
            virtual void setDBPath(const std::string&) = 0; 
    };


}


#endif