#include <fstream>
#include <grpcpp/security/credentials.h>
#include <iostream>
#include "../Implementations/YaController.h"
#include "User.h"

void to_bin(Entity::Track& ent)
{
    std::ofstream output_file("1.mp3", std::ios::binary);
    output_file.write(ent.get_track_bytes(), ent.get_track_bytes_len());
    output_file.close();
}


int main()
{
    Controller::YaController c(new Client(grpc::CreateChannel("localhost:9999", grpc::InsecureChannelCredentials())) );
    Entity::User usr;
    auto tr = c.fetch_track("228", usr);

    to_bin(tr);
    
    return 0;
}