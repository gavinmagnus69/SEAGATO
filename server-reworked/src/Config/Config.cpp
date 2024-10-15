#include "Config.h"

Config::Config(const std::string& configPath) noexcept {
    
    try{
       initConfig(configPath);
    }
    catch(const std::exception& exp){
        spdlog::error("{}, using default config", exp.what());
        initConfig(this->defaultConfigPath);
    }
}

void Config::initConfig(const std::string configPath){
    
    YAML::Node config = YAML::LoadFile(configPath);

    for(auto it = config.begin(); it != config.end(); ++it){
        this->params[it->first.as<std::string>()] = it->second.as<std::string>();
    }

    spdlog::info("Config initialized");
}


std::map<std::string, std::string>& Config::getConfig() {
    return this->params;
}

bool Config::isEmpty() const {
    return this->params.empty();
}

void Config::printConfig() {
    for(const auto& [key, value] : this->params) {
        spdlog::info("{} = {}", key, value);
    }
}

std::optional<std::string> Config::get(const std::string& key) {
    if(this->params.find(key) != this->params.end()){
        try{
            auto val = this->params.at(key);
            return std::optional<std::string>(val);
        }
        catch(...){
            return std::nullopt;
        }
    }
    return std::optional<std::string>();
}

Config::~Config() {}



