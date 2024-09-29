#ifndef CONFIG_H
#define CONFIG_H
#include <yaml-cpp/yaml.h>
#include <iostream>

class Config{
    public:
        std::string port_auth;
        std::string port_audio;
        std::string port_yandex;
        std::string port_tg;
        std::string storage_path;
        int msg_size;

    public:
        Config(const std::string&) noexcept;
        void show_config();
};


#endif