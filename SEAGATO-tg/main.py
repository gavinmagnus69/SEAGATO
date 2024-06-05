import telebot
import os
import bot_token as bp
import threading
import server

bot = telebot.TeleBot(bp.token)

@bot.message_handler(commands = ['start'])
def main(message):
    bot.send_message(message.chat.id, f'This is SEAGATO tgbot, you can send your music here, your chat_id: {message.chat.id}')


@bot.message_handler(content_types = ['audio'])
def get_audio(message):
    bot.reply_to(message, "got it!")
    audio = message.audio

    song_name = message.audio.title
    song_art = message.audio.performer
    full_name = song_art + ' - ' + song_name
    if song_art == "<unknown>":
        full_name = song_name
    file_info = bot.get_file(audio.file_id)
    file = bot.download_file(file_info.file_path)

    dwnld_path = f'./localDB/{message.chat.id}'
    if not os.path.exists(dwnld_path):
        os.makedirs(dwnld_path)
    with open(os.path.join(dwnld_path, f'{full_name}.mp3'), 'wb') as f:
        f.write(file)

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
    thread_1 = threading.Thread(target= tg_handler)
    thread_2 = threading.Thread(target=server.serve)

    thread_2.start()
    thread_1.start()

    thread_1.join()
    thread_2.join()