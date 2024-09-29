#include "Config.h"

Config::Config(const std::string configPath) noexcept {

    YAML::Node config = YAML::LoadFile(configPath);

    if (config["port_auth"]) {
        this->params["port_auth"] = config["port_auth"].as<std::string>();
    }
    if (config["port_audio"]) {
        this->params["port_audio"] = config["port_audio"].as<std::string>();
    }
    if (config["port_yandex"]) {
        this->params["port_yandex"] = config["port_yandex"].as<std::string>();
    }
    if (config["port_tg"]) {
        this->params["port_tg"] = config["port_tg"].as<std::string>();
    }
    if(config["login"]){
        this->params["login"] = config["login"].as<std::string>();
    }
    if(config["password"]){
        this->params["password"] = config["password"].as<std::string>();
    }

    spdlog::info("Config initialized");
}


std::map<std::string, std::string> Config::getConfig() const {
    return this->params;
}

bool Config::isEmpty() const {
    return this->params.empty();
}

void Config::printConfig() {
    for(const auto [key, value] : this->params) {
        spdlog::info("{} = {}", key, value);
    }
}

std::optional<std::string> Config::get(const std::string& key) {
    if(this->params.find(key) != this->params.end()){
        return std::optional<std::string>(this->params[key]);
    }
    return std::optional<std::string>();
}

Config::~Config() {}



