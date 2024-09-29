import telebot
import os
import random
#import bot_token as bp
import threading
#import server
from bot_token import token 

tkn = token

respond = True

bot = telebot.TeleBot(tkn)

@bot.message_handler(commands = ['start'])
def main(message):
    bot.send_message(message.chat.id, f'This is SEAGATO telegram bot, you can send your music here, your chat_id: {message.chat.id}')

@bot.message_handler(commands = ['response'])
def switch_response(message):
    global respond
    if respond == True:
        respond = False
    else:
        respond = True
    bot.send_message(message.chat.id, f'Respond switched to {respond}')


@bot.message_handler(content_types = ['audio'])
def get_audio(message):
    bot.reply_to(message, "got it!")
    audio = message.audio

    song_name = message.audio.title
    song_art = message.audio.performer
    if song_art is None:
        song_art = ""
    if song_name is None:
        song_name = f"{random.randint(0, 999)}"    
    full_name = song_art + ' - ' + song_name
    if song_art == "<unknown>":
        full_name = song_name
    file_info = bot.get_file(audio.file_id)
    file = bot.download_file(file_info.file_path)

    dwnld_path = f'./localDB/{message.chat.id}'
    if not os.path.exists(dwnld_path):
        os.makedirs(dwnld_path)
    try:
        with open(os.path.join(dwnld_path, f'{full_name}.mp3'), 'wb') as f:
            f.write(file)
    except:
        with open(os.path.join(dwnld_path, f'{random.randint(0, 999)}.mp3'), 'wb') as f:
            f.write(file)



#links handler
@bot.message_handler(content_types = ['text'])
def get_from_youtube(message):

    #link to youtube video
    link = message.text

    #bot.reply_to(message, "Processing your link")

    dwnld_path = f'./localDB/{message.chat.id}'
    youtube_path = f'{dwnld_path}/youtube'

    create_folder(dwnld_path)
    create_folder(youtube_path)  
    
    try:
        
        directory_path = './tmp'
        create_folder(directory_path)
        exit_code = download_track(link, directory_path)

        if exit_code != 0:
            bot.reply_to(message, "Unsuc")
            return
        
        if respond:
            for filename in os.listdir(directory_path):
                if os.path.isfile(os.path.join(directory_path, filename)):
                    if '.mp3' in filename:
                        print(filename)
                        bot.send_audio(message.chat.id, audio=open(f'{directory_path}/{filename}', 'rb'))
                        delete_dir(directory_path)
                        
        download_track(link, youtube_path)

        # bot.reply_to(message, "Suc")
    except:
            bot.reply_to(message, "Error")



def create_folder(path: str):
    if not os.path.exists(path):
        os.makedirs(path)

def download_track(link: str, path : str):
    command = f'yt-dlp --audio-quality 0 --audio-format mp3 -x -P {path} {link}'
    exit_code = os.system(command)
    if exit_code != 0:
        return -1
    return 0

def delete_dir(dir_path: str):
    command = f'rm -rf {dir_path}'
    exit_code = os.system(command)
    if exit_code != 0:
        print("Error while removing dir")
        return -1
    return 0

def move_track(file: str, dest: str):
    mv_command = f'mv {file} {dest}'
    exit_code = os.system(mv_command)
    г
    if exit_code != 0:
        print('move error')
        return -1
    return 0
    





@bot.message_handler()
def default_reply(message):
    file = open('./files/who.png', 'rb')
    bot.send_photo(message.chat.id, file)
def tg_handler():
    bot.polling(none_stop= True)

def test():
    i = 0
    while True:
        print(i)
        i += 1


if __name__ == "__main__":
    print('runnging server')
    
    tg_handler()
    #thread_1 = threading.Thread(target= tg_handler)
    #thread_2 = threading.Thread(target=server.serve)

    #thread_2.start()
    #thread_1.start()

    #thread_1.join()
    #thread_2.join()