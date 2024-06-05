#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include "../Interfaces/IDBController.h"
#include "sqlite3.h"
#include <iostream>

namespace Controller {


    class DBController : public Interface::IBDController{
        public:
            int add(const std::vector<std::string>&) override;
            std::variant<std::string, int> find(const std::string&) override;
            int update(const std::vector<std::string>& data) override;
            std::string get_token(const std::string& login) override;
            std::string get_chatid(const std::string& login) override; 
            void setDBPath(const std::string&) override;
        public:
            DBController(const std::string&);
        private:
            std::string db_path;
            sqlite3* db;
            
    };
}
#endif