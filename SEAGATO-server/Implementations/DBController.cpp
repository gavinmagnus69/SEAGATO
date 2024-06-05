#include "DBController.h"
#include <cstddef>
#include <string>

int Controller::DBController::add(const std::vector<std::string>& data){
    //failed to create db: -1
    //failed to insert: -2
    //suc: 0
    if(data.size() == 0){
        return -1;
    }


    //can be changed
    std::string login = data[0];
    std::string chat_id = data[1];
    std::string ytoken = data[2];

    auto chk = this->get_token(login);

    if(chk != "err" && chk != ""){
        
        return -3;
    }

    char* msgErr;
    int exit = sqlite3_open(db_path.c_str(), &this->db);

    std::string sql("INSERT INTO USERS(LOGIN, CHAT_ID, TOKEN) VALUES('"+login+"', '"+chat_id+"', '"+ytoken+"');");
    exit = sqlite3_exec(this->db, sql.c_str(), NULL, 0, &msgErr);
    if (exit != SQLITE_OK){
        std::cout << msgErr << '\n';
        //failed to insert
        return -2;
    }
    else{
        std::cout << "suc insertion\n";
        return 0;
    }
}

std::variant<std::string, int> Controller::DBController::find(const std::string&){
    std::variant<std::string, int> var;
    return var;
}

int Controller::DBController::update(const std::vector<std::string>& data){
    
    if (data.size() == 0){
        return -1;
    }
    
    std::string login = data[0];
    std::string chat_id = data[1];
    std::string ytoken = data[2];

    char* msgErr;
    int exit = sqlite3_open(db_path.c_str(), &this->db);

    std::string sql("UPDATE USERS SET CHAT_ID = '"+chat_id+"', TOKEN = '"+ytoken+"' WHERE LOGIN = '"+login+"';");

    if(chat_id == "nil" && ytoken != "nil"){
        sql = "UPDATE USERS SET TOKEN = '"+ytoken+"' WHERE LOGIN = '"+login+"';";
    }
    if(chat_id != "nil" && ytoken == "nil"){
        sql = "UPDATE USERS SET CHAT_ID = '"+chat_id+"' WHERE LOGIN = '"+login+"';";
    }


    exit = sqlite3_exec(this->db, sql.c_str(), NULL, 0, &msgErr);

    if(exit != SQLITE_OK){
        std::cout << msgErr << "\n";
        sqlite3_free(msgErr);
        sqlite3_close(db);

        return -1; 
    }

    sqlite3_close(db);

    return 0;
} 


Controller::DBController::DBController(const std::string& path){
    db_path = path;
    //init db and table
    int exit = 0;
    char* msgErr;

    exit = sqlite3_open(db_path.c_str(), &this->db);

    if(exit == SQLITE_OK){
        std::cout << "db created\n";
    }
    else{
        std::cout << "could not create db\n";
        return;
    }

    std::string sql = "CREATE TABLE IF NOT EXISTS USERS("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
    "LOGIN TEXT NOT NULL,"
    "CHAT_ID TEXT NOT NULL,"
    "TOKEN TEXT NOT NULL);";

    exit = sqlite3_exec(this->db, sql.c_str(), NULL, 0, &msgErr);

    if(exit != SQLITE_OK){
        std::cout << msgErr;
        return;
    }
    std::cout << "suc\n";

    sqlite3_close(this->db);
}

std::string Controller::DBController::get_token(const std::string& login){
    char* msgErr;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open(db_path.c_str(), &this->db);
    if(exit != SQLITE_OK){
        return "err";
    }

    std::string sql("SELECT TOKEN FROM USERS WHERE LOGIN = '"+login+"';");

    exit = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, NULL);

    if(exit != SQLITE_OK){
        std::cout << msgErr << '\n';
        sqlite3_free(msgErr);
        sqlite3_close(this->db);
        return "err";
    }
    std::string ans;
    while(sqlite3_step(stmt) == SQLITE_ROW){

        // :)
        ans = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return ans;
}


std::string Controller::DBController::get_chatid(const std::string& login){
    char* msgErr;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open(db_path.c_str(), &this->db);
    if(exit != SQLITE_OK){
        return "err";
    }

    std::string sql("SELECT CHAT_ID FROM USERS WHERE LOGIN = '"+login+"';");

    exit = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, NULL);

    if(exit != SQLITE_OK){
        std::cout << msgErr << '\n';
        sqlite3_free(msgErr);
        sqlite3_close(this->db);
        return "err";
    }
    std::string ans;
    while(sqlite3_step(stmt) == SQLITE_ROW){

        // :)
        ans = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return ans;
} 

void Controller::DBController::setDBPath(const std::string& path){
    this->db_path = path;
}

