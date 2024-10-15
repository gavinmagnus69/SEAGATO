#ifndef DATABASE_H
#define DATABASE_H

#include "IDatabase.h"
#include "Config/IConfig.h"
#include <memory>
#include <pqxx/pqxx>
#include "spdlog/spdlog.h"


class Database : public IDatabase {
private:
    std::shared_ptr<IConfig> cfg;
    std::shared_ptr<pqxx::connection> connection;
public:
    Database(std::shared_ptr<IConfig> cfg);
    ~Database() override;   
public:
    bool connect() override;
    bool disconnect() override;
    bool insert(const std::string& table, const std::map<std::string, std::string>& data) override;
    bool remove(const std::string& table, const std::map<std::string, std::string>& conditions) override;
    std::optional<std::vector<std::map<std::string, std::string>>> get(const std::string& table, const std::map<std::string, std::string>& conditions) override;
    bool update(const std::string& table, const std::map<std::string, std::string>& values, const std::map<std::string, std::string>& conditions) override;
};
#endif