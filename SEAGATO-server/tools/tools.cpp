#include "tools.h"


std::string tools::get_login(const std::string& token){
    try {
        auto decoded = jwt::decode(token);
        for(auto& e : decoded.get_payload_json()){
            if(e.first == "email"){
                return e.second.to_str();
            }
        }
        return "";
    } catch (...) {
        std::cout << token << '\n';
        return "";
    }
    
}