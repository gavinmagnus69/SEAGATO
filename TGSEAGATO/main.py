import telebot
import os
import bot_token as bp
import mutagen

bot = telebot.TeleBot(bp.token)


@bot.message_handler(commands = ['start'])
def main(message):
    bot.send_message(message.chat.id, 'This is SEAGATO tgbot, you can send your music here')


@bot.message_handler(content_types = ['audio'])
def get_audio(message):
    bot.reply_to(message, "got it!")
    audio = message.audio
    file_info = bot.get_file(audio.file_id)
    file = bot.download_file(file_info.file_path)

    with open(os.path.join('./localDB', f'{audio.file_id}.m4a'), 'wb') as f:
        f.write(file)



@bot.message_handler()
def default_reply(message):
    file = open('./files/who.png', 'rb')
    bot.send_photo(message.chat.id, file)

bot.polling(none_stop = True)
