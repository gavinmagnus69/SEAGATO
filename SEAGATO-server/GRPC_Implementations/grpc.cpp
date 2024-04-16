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
        std::cout << status.error_code() << ":" << status.error_message() << std::endl;
        return "";
     }
}

 void Client::getTracksByStream(const std::vector<std::string>& request, std::vector<std::string>& response )
        {
            ClientContext context;
             ::Tracks_list req;

             ::Batch batch;
             for(const auto& str : request)
            {
                req.add_tracks_names(str);
            }

            std::unique_ptr<grpc::ClientReader<Batch>> reader(
                _stub->SendTracksButched(&context, req));

            std::string tmp = "";    
            while (reader->Read(&batch)) 
            {
                if (batch.code() == 0){
                     response.emplace_back(batch.data());
                }
                if (batch.code() == 1){
                    tmp += std::string(batch.data());
                }
                if (batch.code() == 2){
                    response.push_back(tmp);
                    tmp = "";
                }
            }   

            Status status = reader->Finish();
            if(status.ok())
            {
                std::cout << "everything is ok\n";
            }
            else
            {
                std::cout << status.error_code() << ":" << status.error_message() << std::endl;
                std::cout << "something went wrong\n";
            } 


        }