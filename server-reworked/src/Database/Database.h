#ifndef DATABASE_H
#define DATABASE_H

#include "IDatabase.h"
#include "IConfig.h"
#include <memory>

class Database : public IDatabase {
private:
    std::shared_ptr<IConfig> cfg;
public:
    Database(const std::shared_ptr<IConfig>& cfg);
    ~Database() override;   
public:
    // bool connect(const std::string&, );
    bool disconnect();
    bool insert(const std::string& table, const std::map<std::string, std::string>& data) override;
    bool remove(const std::string& table, const std::map<std::string, std::string>& conditions) override;
    std::optional<std::map<std::string, std::string>> get(const std::string& table, const std::map<std::string, std::string>& conditions) override;
    bool update(const std::string& table, const std::map<std::string, std::string>& values, const std::map<std::string, std::string>& conditions) override;
};
#endif