# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc
import warnings

import audio_pb2 as audio__pb2

GRPC_GENERATED_VERSION = '1.63.0'
GRPC_VERSION = grpc.__version__
EXPECTED_ERROR_RELEASE = '1.65.0'
SCHEDULED_RELEASE_DATE = 'June 25, 2024'
_version_not_supported = False

try:
    from grpc._utilities import first_version_is_lower
    _version_not_supported = first_version_is_lower(GRPC_VERSION, GRPC_GENERATED_VERSION)
except ImportError:
    _version_not_supported = True

if _version_not_supported:
    warnings.warn(
        f'The grpc package installed is at version {GRPC_VERSION},'
        + f' but the generated code in audio_pb2_grpc.py depends on'
        + f' grpcio>={GRPC_GENERATED_VERSION}.'
        + f' Please upgrade your grpc module to grpcio>={GRPC_GENERATED_VERSION}'
        + f' or downgrade your generated code using grpcio-tools<={GRPC_VERSION}.'
        + f' This warning will become an error in {EXPECTED_ERROR_RELEASE},'
        + f' scheduled for release on {SCHEDULED_RELEASE_DATE}.',
        RuntimeWarning
    )


class AudioServicesStub(object):
    """Missing associated documentation comment in .proto file."""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.sendAudio = channel.unary_unary(
                '/AudioServices/sendAudio',
                request_serializer=audio__pb2.Request.SerializeToString,
                response_deserializer=audio__pb2.Audio.FromString,
                _registered_method=True)
        self.sendTrackList = channel.unary_unary(
                '/AudioServices/sendTrackList',
                request_serializer=audio__pb2.tracksListRequest.SerializeToString,
                response_deserializer=audio__pb2.Tracks_list.FromString,
                _registered_method=True)
        self.SendTracksButched = channel.unary_stream(
                '/AudioServices/SendTracksButched',
                request_serializer=audio__pb2.tracksRequest.SerializeToString,
                response_deserializer=audio__pb2.Batch.FromString,
                _registered_method=True)


class AudioServicesServicer(object):
    """Missing associated documentation comment in .proto file."""

    def sendAudio(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def sendTrackList(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def SendTracksButched(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_AudioServicesServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'sendAudio': grpc.unary_unary_rpc_method_handler(
                    servicer.sendAudio,
                    request_deserializer=audio__pb2.Request.FromString,
                    response_serializer=audio__pb2.Audio.SerializeToString,
            ),
            'sendTrackList': grpc.unary_unary_rpc_method_handler(
                    servicer.sendTrackList,
                    request_deserializer=audio__pb2.tracksListRequest.FromString,
                    response_serializer=audio__pb2.Tracks_list.SerializeToString,
            ),
            'SendTracksButched': grpc.unary_stream_rpc_method_handler(
                    servicer.SendTracksButched,
                    request_deserializer=audio__pb2.tracksRequest.FromString,
                    response_serializer=audio__pb2.Batch.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'AudioServices', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class AudioServices(object):
    """Missing associated documentation comment in .proto file."""

    @staticmethod
    def sendAudio(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(
            request,
            target,
            '/AudioServices/sendAudio',
            audio__pb2.Request.SerializeToString,
            audio__pb2.Audio.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)

    @staticmethod
    def sendTrackList(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(
            request,
            target,
            '/AudioServices/sendTrackList',
            audio__pb2.tracksListRequest.SerializeToString,
            audio__pb2.Tracks_list.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)

    @staticmethod
    def SendTracksButched(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_stream(
            request,
            target,
            '/AudioServices/SendTracksButched',
            audio__pb2.tracksRequest.SerializeToString,
            audio__pb2.Batch.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)