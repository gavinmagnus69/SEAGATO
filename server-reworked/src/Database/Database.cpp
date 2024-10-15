#include "Database.h"

#include <iostream>

Database::Database(std::shared_ptr<IConfig> cfg) : cfg(cfg) {
    connect();
}

//TODO: sql injection-safety
std::optional<std::vector<std::map<std::string, std::string>>> Database::get(const std::string& table, const std::map<std::string, std::string>& conditions){
    
    
    std::string sql;
    
    if(conditions.empty()){
        sql = std::format("SELECT * FROM \"{}\" LIMIT 5", table);
    }
    else {
        //TODO: commas
        std::string cond;
        int ands = conditions.size() - 1;
        for(const auto& [key, value] : conditions){
            cond += std::format("{} = \'{}\'", key, value);
            if(!ands){
                continue;
            }
            cond += " AND ";
            --ands;
        }
        sql = std::format("SELECT * FROM \"{}\" WHERE {} LIMIT 5", table, cond);

    }

    pqxx::nontransaction n(*connection.get());

    pqxx::result R(n.exec(sql));
    
    
    if(R.empty()){
        return std::nullopt;
    }

    std::vector<std::map<std::string, std::string>> ans;
    
    for (const auto& row : R) {
        std::map<std::string, std::string> r;
        for(const auto& elem : row){
            r[R.column_name(elem.num())] = elem.as<std::string>();
        }
        ans.push_back(r);
    
    }
    return ans;
}


bool Database::connect(){


    if(cfg->get("database").has_value() && cfg->get("login").has_value() && cfg->get("password").has_value() && cfg->get("host").has_value()){

        std::string con = std::format("dbname={} user={} password={} host={}", cfg->get("database").value(), cfg->get("login").value(), cfg->get("password").value(), cfg->get("host").value());
        connection = std::make_shared<pqxx::connection>(con);
        return true;
    }

    return false;
}

bool Database::disconnect(){
    connection->close();
    return true;
}

Database::~Database(){
    disconnect();
    // connection->close();
}   


bool Database::insert(const std::string& table, const std::map<std::string, std::string>& data) {
    if(data.empty()){
        return false;
    }

    std::string columns;
    std::string values;

    int commas = data.size() - 1;

    for(const auto& [key, value] : data){
        columns += key;
        values += std::format("'{}'", value);
        if(!commas){
            continue;
        }
        columns += ",";
        values += ",";
        --commas;
    }

    std::string sql = std::format("INSERT INTO \"{}\" ({}) VALUES ({})", table, columns, values);

    spdlog::info("{}", sql);

    pqxx::nontransaction n(*connection.get());
    
    pqxx::result R(n.exec(sql));

    return true;
}

bool Database::remove(const std::string& table, const std::map<std::string, std::string>& conditions) {
    
    //TODO: commas
    std::string cond;
    int ands = conditions.size() - 1;
    for(const auto& [key, value] : conditions){
        cond += std::format("{} = \'{}\'", key, value);
        if(!ands){
                continue;
        }
        cond += " AND ";
        --ands;
    }
    std::string sql = std::format("DELETE FROM \"{}\" WHERE {}", table, cond);

    pqxx::nontransaction n(*connection.get());
    
    pqxx::result R(n.exec(sql));
    
    return true;
}
bool Database::update(const std::string& table, const std::map<std::string, std::string>& values, const std::map<std::string, std::string>& conditions) {
    
    std::string values;
    std::string cond;

    
    
    return true;
}
