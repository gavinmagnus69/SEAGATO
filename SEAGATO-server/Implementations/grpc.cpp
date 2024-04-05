#include "grpc.h"
#include "audio.grpc.pb.h"
#include "audio.pb.h"
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/support/status.h>
#include <memory>
#include <string>



Client::Client(std::shared_ptr<Channel> channel) :
    _stub(AudioServices::NewStub(channel)){}

std::string Client::sendData(std::string song_id, Entity::User& usr)
{
    ::Request req;
    req.set_song_id(song_id);

    ::Audio response;
    
    grpc::ClientContext context;

    grpc::Status status = _stub->sendAudio(&context,
     req, &response);

     if(status.ok())
     {
        return response.data();
     }
     else
     {

        std::cout << "error in grpc client\n";
        return "";
     }
}