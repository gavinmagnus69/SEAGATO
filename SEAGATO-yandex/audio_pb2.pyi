from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Optional as _Optional

DESCRIPTOR: _descriptor.FileDescriptor

class Request(_message.Message):
    __slots__ = ("song_id",)
    SONG_ID_FIELD_NUMBER: _ClassVar[int]
    song_id: str
    def __init__(self, song_id: _Optional[str] = ...) -> None: ...

class Audio(_message.Message):
    __slots__ = ("data",)
    DATA_FIELD_NUMBER: _ClassVar[int]
    data: bytes
    def __init__(self, data: _Optional[bytes] = ...) -> None: ...

class Track(_message.Message):
    __slots__ = ("track_name", "data", "song_id")
    TRACK_NAME_FIELD_NUMBER: _ClassVar[int]
    DATA_FIELD_NUMBER: _ClassVar[int]
    SONG_ID_FIELD_NUMBER: _ClassVar[int]
    track_name: str
    data: bytes
    song_id: str
    def __init__(self, track_name: _Optional[str] = ..., data: _Optional[bytes] = ..., song_id: _Optional[str] = ...) -> None: ...

class Request_list(_message.Message):
    __slots__ = ("req",)
    REQ_FIELD_NUMBER: _ClassVar[int]
    req: bool
    def __init__(self, req: bool = ...) -> None: ...

class Tracks_list(_message.Message):
    __slots__ = ("tracks_names",)
    TRACKS_NAMES_FIELD_NUMBER: _ClassVar[int]
    tracks_names: _containers.RepeatedScalarFieldContainer[str]
    def __init__(self, tracks_names: _Optional[_Iterable[str]] = ...) -> None: ...

class User(_message.Message):
    __slots__ = ("yandex_token", "tg_id")
    YANDEX_TOKEN_FIELD_NUMBER: _ClassVar[int]
    TG_ID_FIELD_NUMBER: _ClassVar[int]
    yandex_token: str
    tg_id: str
    def __init__(self, yandex_token: _Optional[str] = ..., tg_id: _Optional[str] = ...) -> None: ...

class Batch(_message.Message):
    __slots__ = ("track_name", "data", "code")
    TRACK_NAME_FIELD_NUMBER: _ClassVar[int]
    DATA_FIELD_NUMBER: _ClassVar[int]
    CODE_FIELD_NUMBER: _ClassVar[int]
    track_name: str
    data: bytes
    code: int
    def __init__(self, track_name: _Optional[str] = ..., data: _Optional[bytes] = ..., code: _Optional[int] = ...) -> None: ...
