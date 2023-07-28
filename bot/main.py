import os
import requests
from dotenv import load_dotenv
from telegram import Update, ReplyKeyboardMarkup
from telegram.ext import ApplicationBuilder, CommandHandler, MessageHandler, ContextTypes, filters


load_dotenv()
TOKEN = os.getenv("TOKEN")
isSetAmount = False

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    await update.message.reply_text('Hello this is bot for manage your automatic feeder!!!', reply_markup=get_keyboard())

async def ask_for_number(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    global isSetAmount
    isSetAmount = True       
    response = requests.get("http://192.168.0.14:80/get_feedamount")
    response.raise_for_status()  # Raise an exception for 4xx and 5xx status codes
    await update.message.reply_text(f"Current Amount Feed: {response.text}\nPlease enter a amount feed:")

async def handle_number(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    global isSetAmount
    if isSetAmount:
        try:
            number = int(update.message.text)
            data = str(number)
            response = requests.post("http://192.168.0.14:80/change_feedamount", data=data)
            result = response.text
            await update.message.reply_text(result)
        except ValueError:
            await update.message.reply_text("Invalid input. Please enter a valid number.")
        finally: 
            isSetAmount = False
            
async def github(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    await update.message.reply_text('https://github.com/hailrakeqq', reply_markup=get_keyboard())

async def language(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    await update.message.reply_text('here you can change language... coming soon', reply_markup=get_keyboard())

async def feed(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    try:
        response = requests.get("http://192.168.0.14:80/feed")
        response.raise_for_status()  # Raise an exception for 4xx and 5xx status codes
        return response.text
    except requests.exceptions.RequestException as e:
        await update.message.reply_text(f"An error occurred: {e}", reply_markup=get_keyboard())
        return None


def get_keyboard():
    keyboard = [
        ["/setamount", "/feed"],
        ["/language", "/github"]
    ]
    return ReplyKeyboardMarkup(keyboard, one_time_keyboard=True, resize_keyboard=True)


def main():
    app = ApplicationBuilder().token(TOKEN).build()
    handlers = [CommandHandler("start", start), 
                CommandHandler("setamount", ask_for_number),
                CommandHandler("feed", feed),
                CommandHandler("github", github),
                CommandHandler("language", language),
                MessageHandler(filters.TEXT & ~filters.COMMAND, handle_number)
                ]
    app.add_handlers(handlers)

    print("bot has been started")
    app.run_polling(allowed_updates=Update.ALL_TYPES)

main()