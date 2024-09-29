#include "spdlog/spdlog.h"
#include "Config/Config.h"

int main() {
    try{
        std::string configPath = "/home/roman/Desktop/personal/projects/SEAGATO/server-reworked/Config/app.yaml";

        std::shared_ptr<IConfig> cfg(new Config(configPath));

        cfg->printConfig();
         
    }
    catch(const std::exception& exp) {
        spdlog::error(exp.what());
    }
}