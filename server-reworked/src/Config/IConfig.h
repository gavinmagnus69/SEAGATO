#ifndef ICONFIG_H
#define ICONFIG_H
#include <optional>
#include <string>
#include <map>

class IConfig {
public:
    virtual std::optional<std::string> get(const std::string& key) = 0;
    virtual std::map<std::string, std::string>& getConfig() = 0;
    virtual bool isEmpty() const = 0;
    virtual void printConfig() = 0;
    virtual ~IConfig() {};
};
#endif