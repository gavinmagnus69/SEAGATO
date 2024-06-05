#include <cstddef>
#include <cstdint>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include "proto/hello.grpc.pb.h"
#include "proto/hello.pb.h"
#include "proto/sso.grpc.pb.h"
#include "proto/sso.pb.h"
#include "proto/auth.pb.h"
#include "proto/auth.grpc.pb.h"
#include <grpcpp/support/status.h>
#include <grpcpp/support/sync_stream.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <jwt-cpp/jwt.h>
#include <sqlite3.h>
#include <yaml-cpp/yaml.h>
#include <chrono>


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
//FUNCTION that converts string to binary file

void yamler(){
    YAML::Node config = YAML::LoadFile("config.yaml");

    if (config["lastlogin"]) {
    std::cout << "Last logged in: " << config["lastlogin"].as<std::string>() << "\n";
    }
}
void to_bin(const std::string& name, char* bytes, int sz)
{

    std::ofstream output_file("./music/" + name + ".mp3", std::ios::binary);
    output_file.write(bytes, sz);
    output_file.close();

}


void vector_to_bin(std::vector<std::string> &sngs,
                   std::vector<std::string> &bytes_tracks) {
    int i = 0;
    for (auto it = bytes_tracks.begin(); it != bytes_tracks.end(); ++it) {
        to_bin(sngs[i], it->data(), it->length());
        ++i;
    }
}

class Client{
    public:
        Client(std::shared_ptr<Channel> channel) :
        stub_audio(AudioServices::NewStub(channel)) {}

    public:

        //gets single track
        std::string sendAudio(std::string song_id){
            
            ::Request req;
            req.set_song_id(song_id);

            ::Audio response;

            ::grpc::ClientContext context;

            ::grpc::Status status = stub_audio->sendAudio(&context, req, &response);

            if(status.ok()){
                //std::cout << response.data();
                return response.data();
            }   else{
                std::cout << status.error_code() << ":" << status.error_message() << std::endl;
            }
            return "";
        }


 
        
        //gets names of tracks
        void getList(const std::string& token, std::vector<std::string>& tracks, bool tp)
        {
            ::tracksListRequest req;
            req.set_type(tp);
            req.set_token(token);
            ::Tracks_list res;
            ::grpc::ClientContext context;
            ::grpc::Status status = stub_audio->sendTrackList(&context, req, &res);
            if(status.ok()){


                //std::cout << response.data();
                for(const auto& str : res.tracks_names())
                {
                    tracks.push_back(str);
                }
            }   else{
                std::cout << status.error_code() << ":" << status.error_message() << std::endl;
            }
        }


        void getTracksByBatch( const std::string& tkn, std::vector<std::string>& request, std::vector<std::string>& response, bool tp)
        {
            ClientContext context;
             ::tracksRequest req;
             req.set_token(tkn);

             ::Batch batch;

             req.set_type(tp);
             for(const auto& str : request)
            {
                req.add_tracks_names(str);
            }

            std::unique_ptr<grpc::ClientReader<Batch>> reader(
                stub_audio->SendTracksButched(&context, req));

            request.clear();
            std::string tmp = "";    
            int counter = 0;
            while (reader->Read(&batch)) 
            {
                if (batch.code() == 0){
                    //std::cout << "batch code 0\n";
                     response.emplace_back(batch.data());
                     request.push_back(batch.track_name());
                     tmp = "";
                }
                if (batch.code() == 1){
                    //std::cout << "batch code 1\n";
                    tmp += std::string(batch.data());
                    std::string tps = std::string(batch.data());
                    to_bin("batches"+std::to_string(counter)+"", tps.data(), tps.size());
                    counter++;
                    
                }
                if (batch.code() == 2){
                    //std::cout << "batch code 2\n";
                    response.push_back(tmp);
                    request.push_back(batch.track_name());
                    tmp = "";
                }
                if(batch.code() == -1){
                    std::cout << "class client; getTracksByBatch; error happend: -1 code\n";
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
        
        
    private:
        //std::unique_ptr<ProcessingServices::Stub> stub_;
        std::unique_ptr<AudioServices::Stub> stub_audio;
};


class AuthClient{
    private:
        std::unique_ptr<auth::Auth::Stub> _stub;
        int default_app_id = 1;
    public:
        AuthClient(std::shared_ptr<Channel> channel) : 
        _stub(auth::Auth::NewStub(channel)){}
    public:
        //Register RegRequest RegResponse
        std::variant<int64_t, std::string> Register(const std::string& login, const std::string& password){
            ClientContext context;
            auth::RegisterRequest request;
            request.set_email(login);
            request.set_password(password);
            auth::RegisterResponse response;
            ::grpc::Status status = _stub->Register(&context, request, &response);
            std::variant<int64_t, std::string> ans;
            if(status.ok()){
                std::cout << "user registered\n";
                ans = response.user_id();
                return ans;
            }
            else{
                ans = std::to_string(status.error_code()) + ' ' + status.error_message();
                return ans;
            }
        }

        std::variant<std::string, int> Login(const std::string& login, const std::string& pass){
            ClientContext context;
            auth::LoginRequest request;
            auth::LoginResponse resp;
            request.set_email(login);
            request.set_password(pass);
            request.set_app_id(default_app_id);

            ::grpc::Status status = _stub->Login(&context, request, &resp);
            std::variant<std::string, int> ans;

            if(status.ok()){
                std::cout << "succesfull login\n";
                ans = resp.token();
            }
            else{
                std::cout << "failed to log in\n";
                ans = 0;
            }

            return ans;
        }
          
};




class AuthToServer{
    private:
        std::unique_ptr<AuthServices::Stub> _stub;
    public:
        AuthToServer(std::shared_ptr<Channel> channel) :
        _stub(AuthServices::NewStub(channel)) {}
    public:

        int firstLogin(const std::string& token){
            ClientContext context;
            ::addUserRequest request;
            request.set_token(token);
            ::addUserResponse response;

            ::grpc::Status status = _stub->addUser(&context, request, &response);

            if(status.ok()){
                std::cout << "response recieved\n";
                return response.code();
            }
            else{
                std::cout << "response did not recieved "+status.error_message()+" and "+std::to_string(status.error_code())+"\n";
                return -1;
            }
        }    
};




void sso(){
    AuthClient auther(grpc::CreateChannel("localhost:44044", grpc::InsecureChannelCredentials()));

    while(true){
        std::cout << "\nchoose log in or register new user:\n";
        std::cout << "\n1 - registration\n2 - log in\n";
        int ch = 0;
        std::cin >> ch;
        switch (ch) {
        
            case 1: {
                std::cout << "enter your login:\n";
                std::string log;
                std::cin >> log;
                std::cout << "\nenter your password:\n";
                std::string pass;
                std::cin >> pass;
                std::variant<int64_t, std::string> ans = auther.Register(log, pass);
                if(std::holds_alternative<int64_t>(ans)){
                    std::cout << "successful registration\n";
                }
                else{
                    std::cout << "something went wrong\n";
                    std::cout << std::get<std::string>(ans) << '\n';
                }
                break;
            }            
            case 2: {
                std::cout << "enter your login\n";
                std::string login;
                std::cin >> login;
                std::cout << "\nenter your password\n";
                std::string password;
                std::cin >> password;
                std::variant<std::string, int> ans = auther.Login(login, password);
                if(std::holds_alternative<std::string>(ans)){
                    std::cout << std::get<std::string>(ans); 
                }else{
                    std::cout << "something went wrong\n";
                }
                break;
            }
            defaulut: {
                std::cout << "vi pidor\n";
                break;
            }

        }
        

    }

}


void db_test(){
    sqlite3* db;
    int exit = 0;

    exit = sqlite3_open("./storage/test.db", &db);

    if(exit == SQLITE_OK){
        std::cout << "created\n";
    }
    else{
        std::cout << "error";
    }

    std::string sql = "CREATE TABLE IF NOT EXISTS USERS("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
    "LOGIN TEXT NOT NULL,"
    "CHAT_ID TEXT NOT NULL,"
    "TOKEN TEXT NOT NULL);";
    char* msgerr;
    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &msgerr);
    if(exit != SQLITE_OK){
        std::cout << "error creating table " << msgerr;
    }
    else{
        std::cout << "suc";
    }

    sqlite3_close(db);
}

void insert_data(const std::string& login, const std::string& chat_id, const std::string& token){
    sqlite3* db;
    char* msgerr;
    int exit = sqlite3_open("./storage/test.db", &db);

    std::string sql("INSERT INTO USERS(LOGIN, CHAT_ID, TOKEN) VALUES('"+login+"', '"+chat_id+"', '"+token+"');");

    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &msgerr);
    if(exit != SQLITE_OK){
        std::cout << msgerr;
    }
    else{
        std::cout << "insert suc\n";
    }
    sqlite3_close(db);

}



int main() {
    //1488 MAIN SERVER
    //
    //db_test();
    //db_test();
   //insert_data("b3brina", "228", "aboba");
    
    sso();
    std::string token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhcHBfaWQiOjEsImVtYWlsIjoicm9tYWJybyIsImV4cCI6MTcxNDQ2OTgxOCwidWlkIjozfQ.yF5luYD5lqAZYBvv6tk8ZkZuR2gTCap7VbJK5LhtyJ8";
    // AuthToServer cli(grpc::CreateChannel("localhost:1489", grpc::InsecureChannelCredentials()));
    // int code = cli.firstLogin("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhcHBfaWQiOjEsImVtYWlsIjoicm9tYWJybyIsImV4cCI6MTcxNDQ2OTgxOCwidWlkIjozfQ.yF5luYD5lqAZYBvv6tk8ZkZuR2gTCap7VbJK5LhtyJ8");
    // std::cout << code;
    //yamler();
    if(false){
        Client client(grpc::CreateChannel("localhost:1488", grpc::InsecureChannelCredentials()));
        std::vector<std::string> sngs;

        std::vector<std::string> bytes_tracks;

        client.getList(token, sngs, true);

        std::vector<std::string> potic{sngs[0], sngs[1], sngs[25]};
        client.getTracksByBatch(token ,potic, bytes_tracks, true);
       
        vector_to_bin(potic, bytes_tracks);
       
    }
    return 0;
    
}