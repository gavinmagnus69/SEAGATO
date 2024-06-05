import grpc
import audio_pb2_grpc
import audio_pb2
import os
from concurrent import futures


def read_binary_file(file_path):
    with open(file_path, 'rb') as file:
        byte_array = file.read()
    return byte_array

def divide_bytes(data, chunk_size):
    divided_chunks = [data[i:i + chunk_size] for i in range(0, len(data), chunk_size)]
    return divided_chunks


class AudioService(audio_pb2_grpc.AudioServicesServicer):
    def sendTrackList(self, request, context):
        
        #token in this case is chat_id
        chat_id = request.token
        
        response = audio_pb2.Tracks_list()
        open_path = f'./localDB/' + chat_id
        if not os.path.exists(open_path):
            return response

        for filename in os.listdir(open_path):
            
            response.tracks_names.append(filename[:-4])
        return response
    def SendTracksButched(self, request, context):
        
        tr_names = request.tracks_names
        tr_bytes = []
        chat_id = request.token
        open_path = f'./localDB/' + chat_id

        resp = audio_pb2.Batch()
        if not os.path.exists(open_path):
            resp.code = -1
            yield resp
        else:
            for filename in os.listdir(open_path):
                track_name = filename[:-4]
                bts = read_binary_file(open_path + '/' + track_name + '.mp3')
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
                #os.remove('./localDB/' + track_name + '.mp3')


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    audio_pb2_grpc.add_AudioServicesServicer_to_server(AudioService(), server)
    server.add_insecure_port("localhost:2889")
    server.start()
    server.wait_for_termination()

if __name__ == "__main__":
    print("server starts")
    serve()