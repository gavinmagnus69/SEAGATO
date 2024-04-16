#include "YaController.h"
#include "Track.h"
#include "User.h"
#include <string>
#include <cstring>
#include <vector>

Controller::YaController::YaController(Interface::IClient* cli, std::string ip_str) :
_client(cli), _ip_port(ip_str)
{}

//TODO REWORK
Entity::Track Controller::YaController::fetch_track(std::string song_id, Entity::User& usr)
{
    std::string response = this->_client->sendData(song_id, usr);
    //TODO REWORK govno 
   
    //delete[] tmp;
    return Entity::Track(song_id, 228, response, response.size());
}

std::vector<Entity::Track> Controller::YaController::fetch_tracks_batches(const std::vector<std::string>& track_names)
{
    std::vector<std::string> bytes;
    this->_client->getTracksByStream(track_names, bytes);

    std::vector<Entity::Track> tracks;
    int i = 0;
    for (const std::string& byt : bytes)
    {
        tracks.emplace_back(track_names[i], 228 ,byt, byt.size()); 
        ++i;
    }

    return tracks;
}
//TODO
int Controller::YaController::connect(std::string ip_port, Entity::User& usr) { return 0;}

//TODO
int Controller::YaController::disconnect(Entity::User& usr){ return 0; }

//TODO
bool Controller::YaController::is_connected(){ return 0; }

// std::list<Entity::Track> fetch_tracks(std::list<std::string>& song_names, Entity::User& cur_usr)
// {

// }