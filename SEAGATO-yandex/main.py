from yandex_music import Client
import tolkien as tkn

def main():
    client = Client(tkn.token)
    client.init()
    tracks = client.users_likes_tracks().fetch_tracks()
    cnt = 0
    for i in tracks:
        artists_name = ""
        for art in i.artists:
            artists_name += art.name.title()
            artists_name += ' '
        song_name = i.title.title()
        i.download(f"./localDB/{song_name}-{artists_name}.mp3", 'mp3', 320)
        if cnt == 10:
            break
        cnt += 1


if __name__ == '__main__':
    main()
