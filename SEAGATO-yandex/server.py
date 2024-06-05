import os
from concurrent import futures
from yandex_music import Client
import time

import grpc
import audio_pb2_grpc
import audio_pb2


def read_binary_file(file_path):
    with open(file_path, 'rb') as file:
        byte_array = file.read()
    return byte_array

def divide_bytes(data, chunk_size):
    divided_chunks = [data[i:i + chunk_size] for i in range(0, len(data), chunk_size)]
    return divided_chunks
class AudioService(audio_pb2_grpc.AudioServicesServicer):
    def sendAudio(self, request, context):
        bytes = read_binary_file(f'./localDB/{request.song_id}.mp3')
        return audio_pb2.Audio(data=bytes)

    def sendTrackList(self, request, context):
        #print('sendTrackList handler')
        token = request.token
        response = audio_pb2.Tracks_list()
        client = Client(token)
        client.init()
        tracks = client.users_likes_tracks().fetch_tracks()
        for i in tracks:
            artists_name = ""
            for art in i.artists:
                artists_name += art.name.title()
                artists_name += ' '
            song_name = i.title.title()
            track_name = song_name + '-' + artists_name
            response.tracks_names.append(track_name)
        return response


    def SendTracksButched(self, request, context):
        #print('sendTracksButched handler')
        tr_names = request.tracks_names
        tr_bytes = []
        token = request.token
        client = Client(token)
        client.init()
        tracks = client.users_likes_tracks().fetch_tracks()

        for track in tracks:
            artists_name = ""
            for art in track.artists:
                artists_name += art.name.title()
                artists_name += ' '
            song_name = track.title.title()
            track_name = song_name + '-' + artists_name
            if track_name in tr_names:
                track.download(f"./localDB/{song_name}-{artists_name}.mp3", 'mp3', 320)
                bts = read_binary_file('./localDB/' + track_name + '.mp3')
                # getting tracks bytes by their names from request
                if len(bts) > 4000000:

                    # chunk size can be changed
                    batches = divide_bytes(bts, 4000000)
                    for b in batches:
                        resp = audio_pb2.Batch()
                        resp.data = b
                        resp.code = 1
                        resp.track_name = track_name
                        yield resp
                    resp = audio_pb2.Batch()
                    resp.data = b'meow'
                    resp.code = 2
                    resp.track_name = track_name
                    yield resp
                else:
                    resp = audio_pb2.Batch()
                    resp.data = bts
                    resp.track_name = track_name
                    resp.code = 0
                    yield resp
                os.remove('./localDB/' + track_name + '.mp3')



def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    audio_pb2_grpc.add_AudioServicesServicer_to_server(AudioService(), server)
    server.add_insecure_port("localhost:2888")
    server.start()
    server.wait_for_termination()


if __name__ == "__main__":
    print("server starts")
    serve()
