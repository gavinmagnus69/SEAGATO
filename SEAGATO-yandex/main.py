from yandex_music import Client

import tolkien
#scipt to download all tracks from yandex music



def main():
    client = Client(tolkien.token)
    client.init()
    tracks = client.users_likes_tracks().fetch_tracks()
    cnt = 0
    for i in tracks:
        artists_name = ""
        for art in i.artists:
            artists_name += art.name.title()
            artists_name += ' '
        song_name = i.title.title()
        try:
            i.download(f"./localDB/{song_name}-{artists_name}.mp3", 'mp3', 320)
        except:
            i.download(f"./localDB/{song_name}-{artists_name}.mp3", 'mp3', 192)



if __name__ == '__main__':
    main()
