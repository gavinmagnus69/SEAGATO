# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: audio.proto
# Protobuf Python Version: 5.26.1
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0b\x61udio.proto\"\x1a\n\x07Request\x12\x0f\n\x07song_id\x18\x01 \x01(\t\"\x15\n\x05\x41udio\x12\x0c\n\x04\x64\x61ta\x18\x01 \x01(\x0c\":\n\x05Track\x12\x12\n\ntrack_name\x18\x01 \x01(\t\x12\x0c\n\x04\x64\x61ta\x18\x02 \x01(\x0c\x12\x0f\n\x07song_id\x18\x03 \x01(\t\"\"\n\x11tracksListRequest\x12\r\n\x05token\x18\x01 \x01(\t\"#\n\x0bTracks_list\x12\x14\n\x0ctracks_names\x18\x01 \x03(\t\"4\n\rtracksRequest\x12\r\n\x05token\x18\x01 \x01(\t\x12\x14\n\x0ctracks_names\x18\x02 \x03(\t\"7\n\x05\x42\x61tch\x12\x12\n\ntrack_name\x18\x01 \x01(\t\x12\x0c\n\x04\x64\x61ta\x18\x02 \x01(\x0c\x12\x0c\n\x04\x63ode\x18\x03 \x01(\x05\x32\x94\x01\n\rAudioServices\x12\x1f\n\tsendAudio\x12\x08.Request\x1a\x06.Audio\"\x00\x12\x33\n\rsendTrackList\x12\x12.tracksListRequest\x1a\x0c.Tracks_list\"\x00\x12-\n\x11SendTracksButched\x12\x0e.tracksRequest\x1a\x06.Batch0\x01\x62\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'audio_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_REQUEST']._serialized_start=15
  _globals['_REQUEST']._serialized_end=41
  _globals['_AUDIO']._serialized_start=43
  _globals['_AUDIO']._serialized_end=64
  _globals['_TRACK']._serialized_start=66
  _globals['_TRACK']._serialized_end=124
  _globals['_TRACKSLISTREQUEST']._serialized_start=126
  _globals['_TRACKSLISTREQUEST']._serialized_end=160
  _globals['_TRACKS_LIST']._serialized_start=162
  _globals['_TRACKS_LIST']._serialized_end=197
  _globals['_TRACKSREQUEST']._serialized_start=199
  _globals['_TRACKSREQUEST']._serialized_end=251
  _globals['_BATCH']._serialized_start=253
  _globals['_BATCH']._serialized_end=308
  _globals['_AUDIOSERVICES']._serialized_start=311
  _globals['_AUDIOSERVICES']._serialized_end=459
# @@protoc_insertion_point(module_scope)