#ifndef ISERVERAUTH_H
#define ISERVERAUTH_H
#include <string>
#include <vector>
class IServerAuth{
    public:
        virtual int addUser(const std::string&) = 0;
        virtual int addUserData(const std::vector<std::string>&) = 0;
        virtual ~IServerAuth(){};
};

#endif