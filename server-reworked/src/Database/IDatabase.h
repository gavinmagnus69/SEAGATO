#ifndef IDATABASE_H
#define IDATABASE_H
#include <string>
#include <map>
#include <optional>
#include <vector>

class IDatabase {
public:
virtual bool connect() = 0;
virtual bool disconnect() = 0;
virtual bool insert(const std::string& table, const std::map<std::string, std::string>& data) = 0;
virtual bool remove(const std::string& table, const std::map<std::string, std::string>& conditions) = 0;
virtual std::optional<std::vector<std::map<std::string, std::string>>> get(const std::string& table, const std::map<std::string, std::string>& conditions) = 0;
virtual bool update(const std::string& table, const std::map<std::string, std::string>& values, const std::map<std::string, std::string>& conditions) = 0;
virtual ~IDatabase(){};    

};

#endif