from concurrent import futures

import time

import grpc
import audio_pb2_grpc
import audio_pb2


def read_binary_file(file_path):
    with open(file_path, 'rb') as file:
        byte_array = file.read()
    return byte_array


class AudioService(audio_pb2_grpc.AudioServicesServicer):
    def sendAudio(self, request, context):
        print(request.song_id)
        reply = audio_pb2.Audio()
        bytes = read_binary_file('./localDB/1.mp3')
        reply.data = bytes
        return audio_pb2.Audio(data=bytes)

    def sendHello(self, request, context):
        print("called")
        reply = audio_pb2.Asnwer()
        reply.ans = "blya"

        print(reply)
        return reply


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    audio_pb2_grpc.add_AudioServicesServicer_to_server(AudioService(), server)
    server.add_insecure_port("localhost:2888")
    server.start()
    server.wait_for_termination()


if __name__ == "__main__":
    print("server starts")
    serve()
