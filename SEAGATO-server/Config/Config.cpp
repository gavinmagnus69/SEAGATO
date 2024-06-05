#include "Config.h"



Config::Config(const std::string& config_path) noexcept{
    YAML::Node config = YAML::LoadFile(config_path);

    if (config["port_auth"]) {
        this->port_auth = config["port_auth"].as<std::string>();
    }
    if (config["port_audio"]) {
        this->port_audio = config["port_audio"].as<std::string>();
    }
    if (config["port_yandex"]) {
        this->port_yandex = config["port_yandex"].as<std::string>();
    }
    if (config["port_tg"]) {
        this->port_tg = config["port_tg"].as<std::string>();
    }
    if (config["storage_path"]) {
        this->storage_path = config["storage_path"].as<std::string>();
    }
    if (config["msg_size"]) {
        this->msg_size = config["msg_size"].as<int>();
    }
}

void Config::show_config(){
    std::cout << ""+this->port_auth+"; "+this->port_audio+"; "+this->port_yandex+"; "+this->port_tg+"; "+this->storage_path+"; " << this->msg_size << '\n'; 
}