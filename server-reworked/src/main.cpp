#include "spdlog/spdlog.h"
#include "Config/Config.h"
#include "Database/Database.h"


int main() {
    try{

        std::string configPath = "../config/app.yaml";

        std::shared_ptr<IConfig> cfg(new Config(configPath));

        // cfg->printConfig();
        
        std::shared_ptr<IDatabase> db(new Database(cfg));
        // db->connect();
        
        db->remove("User", {{"username", "alice"}});
        // db->insert("User", {{"username", "Romabro"}, {"email", "romabro228"}, {"password_hash", "asfasfafaf"}, {"role", "0"}});

        // auto ans = db->get("User", {{"username", "johndoe"}});

        // for(const auto& m : ans.value()){
        //     for(const auto& [key, value] : m){
        //         spdlog::info("{} = {}", key, value);
        //     }
        // }

         
    }
    catch(const std::exception& exp) {
        spdlog::error(exp.what());
    }
}