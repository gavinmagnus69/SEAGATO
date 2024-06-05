
#ifndef ISSO_H
#define ISSO_H

#include <cstdint>
#include <string>
#include <variant>

class Isso{
    public:
        virtual std::variant<std::string, int> Login(const std::string&, const std::string&) = 0;
        virtual std::variant<std::string, int64_t> Register(const std::string&, const std::string&) = 0;
        virtual ~Isso(){};
};


#endif