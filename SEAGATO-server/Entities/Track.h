#ifndef TRACK_H
#define TRACK_H

#include <iostream>
namespace Entity
{
    class Track{
        private:
            //full name?
            std::string track_name;
            //in seconds
            int track_length; 

            char* track_bytes;

            int track_bytes_len;

        public:
            Track(const std::string& name, int length, char* bytes, int len) noexcept;
            Track() noexcept;
            ~Track() noexcept;
        public:
            //getters
            int get_track_length() const;
            std::string get_track_name() const;
            char* get_track_bytes();  
            int get_track_bytes_len() const;

            //setters
            void set_track_length(int len);
            void set_track_name(const std::string& name);
            void set_track_bytes(char* bytes);
            void set_track_bytes_len(int len);
    };

}
#endif