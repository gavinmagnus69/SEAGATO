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
            //ошибочка (надо было string))))
            std::string track_bytes;

            int track_bytes_len;

        public:
            Track(const std::string& name, int length, const std::string& bytes, int len) noexcept;
            Track() noexcept;
            ~Track() noexcept;
        public:
            //getters
            int get_track_length() const;
            std::string get_track_name() const;
            const std::string& get_track_bytes();  
            int get_track_bytes_len() const;
            std::string& data();

            //setters
            void set_track_length(int);
            void set_track_name(const std::string&);
            void set_track_bytes(const std::string&);
            void set_track_bytes_len(int);
    };

}
#endif