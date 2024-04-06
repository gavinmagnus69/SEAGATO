#include "YaController.h"
#include "User.h"
#include <string>
#include <cstring>

Controller::YaController::YaController(Interface::IClient* cli, std::string ip_str) :
_client(cli), _ip_port(ip_str)
{}

//TODO REWORK
Entity::Track Controller::YaController::fetch_track(std::string song_id, Entity::User& usr)
{
    std::string response = _client->sendData(song_id, usr);
    //TODO REWORK govno 
    char* tmp = new char[response.size()];
    memcpy(tmp, response.data(), response.size());
    return Entity::Track(song_id, 228, tmp, response.size());
}

//TODO
int Controller::YaController::connect(std::string ip_port, Entity::User& usr) { return 0;}

//TODO
int Controller::YaController::disconnect(Entity::User& usr){ return 0; }

//TODO
bool Controller::YaController::is_connected(){ return 0; }