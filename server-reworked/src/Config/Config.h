#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <yaml-cpp/yaml.h>
#include <map>
#include "spdlog/spdlog.h"
#include "IConfig.h"

class Config : public IConfig{
private:
    std::map<std::string, std::string> params;
public:
    Config(const std::string configPath) noexcept;
    Config() = default;
    ~Config() override;
public:
    std::map<std::string, std::string> getConfig() const override;
    bool isEmpty() const override;
    void printConfig() override;
    std::optional<std::string> get(const std::string& key) override;
};

#endif