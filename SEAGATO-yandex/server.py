from concurrent import futures

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
    def SendTracksButched(self, request, context):
        tr_names = request.tracks_names
        tr_bytes = []

        #getting tracks bytes by their names from request
        for i in tr_names:
            bytes = read_binary_file('./localDB/' + i + '.mp3')
            tr_bytes.append(bytes)
        index = 0
        for trb in tr_bytes:
            if len(trb) > 4000000:
                # cur_len = len(trb)
                # cur_pos = 0
                # tmp_bytes = None
                # while cur_len > 4000000:
                #     for i in range(4000001):
                #         tmp_bytes[i] = trb[i]
                batches = divide_bytes(trb, 4000000)
                for b in batches:
                    resp = audio_pb2.Batch()
                    resp.data = b
                    resp.code = 1
                    resp.track_name = tr_names[index]
                    yield resp
                resp = audio_pb2.Batch()
                resp.data = b'meow'
                resp.code = 2
                resp.track_name = "morgen"
                yield resp
            else:
                resp = audio_pb2.Batch()
                resp.data = trb
                resp.track_name = tr_names[index]
                resp.code = 0
                yield resp
            index += 1
def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    audio_pb2_grpc.add_AudioServicesServicer_to_server(AudioService(), server)
    server.add_insecure_port("localhost:2888")
    server.start()
    server.wait_for_termination()


if __name__ == "__main__":
    print("server starts")
    serve()
