#ifndef STREAMER_H
#define STREAMER_H

#include <QObject>
#include <QDebug>
#include <memory>
#include <qchar.h>
#include <qmediaplayer.h>
#include <qobject.h>
#include <qthread.h>
#include <string>
#include <fstream>
#include "../Interfaces/IAudioServer.h"
#include "../AudioServer/AudioServer.h"
#include "proto/hello.grpc.pb.h"
#include <QIODevice>
#include <QBuffer>
#include <ui_player.h>

class ByteStreamDevice : public QIODevice
{
    Q_OBJECT

public:
    ByteStreamDevice(QBuffer *buffer, QObject *parent = nullptr)
        : QIODevice(parent), m_buffer(buffer) {}

    bool open(QIODevice::OpenMode mode) override {
        if (!QIODevice::open(mode))
            return false;
        if (m_buffer && !m_buffer->isOpen()) {
            m_buffer->open(QIODevice::ReadOnly);
        }
        return true;
    }

    void close() override {
        if (m_buffer)
            m_buffer->close();
        QIODevice::close();
    }

protected:
    qint64 readData(char *data, qint64 maxSize) override {
        if (!m_buffer)
            return -1;
        qint64 bytesRead = m_buffer->read(data, maxSize);
        return bytesRead;
    }

    qint64 writeData(const char *data, qint64 maxSize) override {
        // This example assumes no data writing
        return -1;
    }

private:
    QBuffer *m_buffer;
};



class StreamerServer {
    private:
        std::unique_ptr<AudioServices::Stub> _stub;
    public:
        StreamerServer(std::shared_ptr<grpc::Channel> channel) : _stub(AudioServices::NewStub(channel)) {}
    public:
        //void getList(const std::string& token, std::vector<std::string>& tracks, bool tp);
        void getTracksByBatch(QMediaPlayer* player ,const std::string& tkn, std::vector<std::string>& request, std::vector<std::string>& response, bool tp){
            grpc::ClientContext context;
            ::tracksRequest req;
            req.set_token(tkn);

            ::Batch batch;

            req.set_type(tp);
            for(const auto& str : request)
            {
                req.add_tracks_names(str);
            }

            std::unique_ptr<grpc::ClientReader<Batch>> reader(
            _stub->SendTracksButched(&context, req));

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

            grpc::Status status = reader->Finish();

            if(status.ok())
            {
                std::cout << "everything is okkkk\n";
            }
            else
            {
                std::cout << status.error_code() << ":" << status.error_message() << std::endl;
                std::cout << "something went wrong\n";
            } 

        }     
};


class Streamer : public QObject {
    Q_OBJECT

public:
    explicit Streamer(QObject *parent = nullptr) : QObject(parent) {
        audio = std::make_shared<StreamerServer>(grpc::CreateChannel("localhost:1488", grpc::InsecureChannelCredentials()));
        _stub = AudioServices::NewStub(grpc::CreateChannel("localhost:1488", grpc::InsecureChannelCredentials()));
    }
private:
    std::shared_ptr<StreamerServer> audio;
    int id = 0;
    std::vector<std::string> bytes;
    std::vector<std::string> tracks_local;
    std::string jwt;
    bool tp;
    QMediaPlayer* player;
    std::unique_ptr<AudioServices::Stub> _stub;

public:
    void to_bin2(const std::string& name, char* bytes, int sz)
    {

        std::ofstream output_file("./Music/" + name + ".mp3", std::ios::binary);
        output_file.write(bytes, sz);
        output_file.close();

    }


    void vector_to_bin2(std::vector<std::string> &sngs,
                    std::vector<std::string> &bytes_tracks) {
        int i = 0;
        for (auto it = bytes_tracks.begin(); it != bytes_tracks.end(); ++it) {
            to_bin2(sngs[i], it->data(), it->length());
            ++i;
        }
    }
    void set_id(int id){
        this->id = id;
    }

    void set_player(QMediaPlayer* player){
        this->player = player;
    }

    void set_args(const std::string& jwt, const std::vector<std::string>& tracks_local, bool tp){
        this->tracks_local.clear();
        this->tracks_local = tracks_local;
        this->jwt = jwt;
        this->tp = tp;
    }    
public slots:
    void doWork() {
        // Simulate a time-consuming task
        bytes.clear();
        
        audio->getTracksByBatch(player, jwt, tracks_local, bytes, true);
        vector_to_bin2(tracks_local, bytes);
        emit workFinished();
    }


    void getTracksByBatch(){
            bytes.clear();
            grpc::ClientContext context;
            ::tracksRequest req;
            req.set_token(jwt);

            ::Batch batch;

            req.set_type(tp);
            for(const auto& str : tracks_local)
            {
                req.add_tracks_names(str);
            }

            std::unique_ptr<grpc::ClientReader<Batch>> reader(
            _stub->SendTracksButched(&context, req));

            tracks_local.clear();
            std::string tmp = "";    
            int counter = 0;
            while (reader->Read(&batch)) 
            {
                if (batch.code() == 0){
                    //std::cout << "batch code 0\n";
                        bytes.emplace_back(batch.data());
                        tracks_local.push_back(batch.track_name());
                        tmp = "";
                }
                if (batch.code() == 1){
                    //std::cout << "batch code 1\n";
                    tmp += std::string(batch.data());
                    std::string tps = std::string(batch.data());
                    
                    counter++;
                    
                }
                if (batch.code() == 2){
                    //std::cout << "batch code 2\n";
                    emit sendBatch(QString::fromStdString(tmp));
                    bytes.push_back(tmp);
                    tracks_local.push_back(batch.track_name());
                    tmp = "";
                }
                if(batch.code() == -1){
                    std::cout << "class client; getTracksByBatch; error happend: -1 code\n";
                }
            }   

            grpc::Status status = reader->Finish();

            if(status.ok())
            {
                std::cout << "everything is okkkk\n";
            }
            else
            {
                std::cout << status.error_code() << ":" << status.error_message() << std::endl;
                std::cout << "something went wrong\n";
            }

        vector_to_bin2(tracks_local, bytes);
        emit workFinished();     

    }     



signals:
    void workFinished();
    void sendBatch(const QString&);
};

#endif 