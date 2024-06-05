#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <jwt-cpp/jwt.h>
#include <iostream>
#include <fstream>



namespace tools{
    //parsing jwt token to extract login
    std::string get_login(const std::string&);

}
#endif