#include "Track.h"
    
    //getters and setters

    Entity::Track::Track(const std::string& name,
     int length, char* bytes, int len) noexcept :
     track_name(name), track_length(length),
     track_bytes(bytes), track_bytes_len(len)
    {}

    Entity::Track::~Track()
    {
        delete [] track_bytes;
    }
    
    Entity::Track::Track() noexcept : track_name("null"), track_length(0){}
    
    int Entity::Track::get_track_length() const
    {
        return this->track_length;
    }

    std::string Entity::Track::get_track_name() const
    {
        return this->track_name;
    }
            
    void Entity::Track::set_track_length(int len)
    {
        this->track_length = len;
    }
    
    void Entity::Track::set_track_name(const std::string& name)
    {
        this->track_name = name;
    }

    void Entity::Track::set_track_bytes(char* bytes)
    {
        this->track_bytes = bytes;
    }

    void Entity::Track::set_track_bytes_len(int len)
    {
        this->track_bytes_len = len;
    }

    char* Entity::Track::get_track_bytes() { return track_bytes;}

    int Entity::Track::get_track_bytes_len() const { return track_bytes_len;}