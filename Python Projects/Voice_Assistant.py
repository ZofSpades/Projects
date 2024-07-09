import subprocess
import wolframalpha
import pyttsx3
import tkinter
import json
import random
import operator
import speech_recognition as sr
import datetime
import wikipedia
import webbrowser
import os
import winshell
import pyjokes
import feedparser
import smtplib
import ctypes
import time
import requests
from clint.textui import progress
from ecapture import ecapture as ec
from bs4 import BeautifulSoup
import win32com.client as wincl
from urllib.request import urlopen
import urllib.parse
import urllib.request

engine = pyttsx3.init('sapi5')
voices = engine.getProperty('voices')
engine.setProperty('voice', voices[1].id)      #use 0 for male voice
current_rate = engine.getProperty('rate')
engine.setProperty('rate', current_rate - 30)

def speak(audio):
	engine.say(audio)
	engine.runAndWait()

def wishMe():
	hour = int(datetime.datetime.now().hour)
	if hour>= 0 and hour<12:
		speak("Good Morning Human!")
	elif hour>= 12 and hour<18:
		speak("Good Afternoon Human!") 
	else:
		speak("Good Evening Human!") 
	assname =("Friday")                    #Don't judge me, I 'am' a Tony Stark fan!
	speak("I am your Assistant!")
	speak(assname)
	

def username():
	speak("What should i call you human?")
	uname = takeCommand()
	speak("Welcome")
	speak(uname)
	print("Welcome", uname)
	speak("How can i Help you?")

def takeCommand():
	r = sr.Recognizer()
	with sr.Microphone() as source:	
		print("Listening...")
		r.pause_threshold = 1
		audio = r.listen(source)
	try:
		print("Recognizing...") 
		query = r.recognize_google(audio, language ='en-in')
		print(f"Human said: {query}\n")
	except Exception as e:
		print(e) 
		print("Unable to recognize your voice.") 
		return "None"
	return query

def query_google(query):
        webbrowser.open(f"google.com/{query}")

def query_wolframalpha(query):
    try:
        client = wolframalpha.Client("KVU42W-X5L6AJLHGJ")
        res = client.query(query, timeout=120)
        if res['@success'] == 'false' or not hasattr(res, 'results'):
            raise ValueError("No results found for the query.")
        results = list(res.results)
        if not results:
            raise StopIteration
        result_text = results[0].text
        print(result_text)
        speak(result_text)
    except StopIteration:
        print("No results found in Wolfram Alpha. Trying Google.")
        speak("I couldn't find any results in Wolfram Alpha. Trying Google.")
        result_text = query_google(query)

if __name__ == '__main__':
	clear = lambda: os.system('cls')
	clear()
	wishMe()
	username()
	while True:
		query = takeCommand().lower()
  
		if 'wikipedia' in query:
			speak('Searching Wikipedia...')
			query = query.replace("wikipedia", "")
			results = wikipedia.summary(query, sentences = 3)
			speak("According to Wikipedia")
			print(results)
			speak(results)
			print("\n")

		elif 'open youtube' in query:
			speak("Here you go to Youtube\n")
			webbrowser.open("youtube.com")

		elif 'open google' in query:
			speak("Here you go to Google\n")
			webbrowser.open("google.com")

		elif 'the time' in query:
			strTime = datetime.datetime.now().strftime("%H:%M:%S") 
			speak(f"Human, the time is {strTime}")

		elif 'how are you' in query:
			speak("I am fine. Thank you for asking.")
			speak("How are you, Human?")

		elif 'fine' in query or "good" in query:
			speak("It's good to know that your fine")

		elif "change my name to" in query:
			query = query.replace("change my name to", "")
			uname = query
			speak("Done!")

		elif "what is my name" in query:
			speak("Your name is")
			speak(uname)

		elif "change your name" in query:
			speak("What would you like to call me? ")
			assname = takeCommand()
			speak("Thanks for naming me")

		elif "what's your name" in query or "What is your name" in query:
			speak("My friends call me")
			speak(assname)
			print("My friends call me", assname)

		elif 'exit' in query:
			speak("Bye bye")
			exit()

		elif "who made you" in query or "who created you" in query: 
			speak("I have been created by another human who had no other work during his vacation.")
			
		elif 'joke' in query:
			speak(pyjokes.get_joke())
			
		elif 'calculate' in query:
			expression = query.split("calculate",1)[1].strip()
			print(f"Expression to evaluate:{expression}")
			result = eval(expression)
			response = f"The result of {expression} is {result}"
			print(response)
			speak(response)

		elif 'search' in query or 'play' in query:
			query = query.replace("search", "") 
			query = query.replace("play", "")		 
			webbrowser.open(f"google.com/{query}") 

		elif "who am i" in query:
			speak(uname)

		elif 'is love' in query:
			speak("It is the 7th sense that destroy all other senses......lol")

		elif "who are you" in query:
			speak("I am your virtual assistant created by another human.")
   
		elif 'change background' in query:
			ctypes.windll.user32.SystemParametersInfoW(20,0,"Location of wallpaper",0)
			speak("Background changed succesfully!")
		
		elif 'lock window' in query:
				speak("Locking the device")
				ctypes.windll.user32.LockWorkStation()

		elif 'shutdown system' in query:
				speak("Hold on a sec! Your system is on its way to shut down.")
				subprocess.call('shutdown / p /f')
				
		elif 'empty recycle bin' in query:
			winshell.recycle_bin().empty(confirm = False, show_progress = False, sound = True)
			speak("Recycle bin is empty.")

		elif "don't listen" in query or "stop listening" in query:
			speak("Tell me number of seconds to stop listening, just the number")
			user_input = takeCommand().strip()
			if user_input.isdigit():
				a = int(user_input)
				time.sleep(a)
				print(a)
			else:
				speak("Sorry, I didn't get the number.")

		elif "where is" in query:
			query = query.lower().replace("where is", "").strip()
			location = query
			speak("Human asked to Locate")
			speak(location)
			webbrowser.open(f"https://maps.google.com/?q={location}")

		elif "take a photo" in query:
			speak("Camera is ready with Friday, say cheeeeeeeese")
			ec.capture(0, "Friday Camera ", "img.jpg")

		elif "restart" in query:
			subprocess.call(["shutdown", "/r"])
			
		elif "hibernate" in query or "sleep" in query:
			speak("Hibernating")
			subprocess.call("shutdown / h")

		elif "log off" in query or "sign out" in query:
			speak("Make sure all the application are closed before sign-out.")
			time.sleep(5)
			subprocess.call(["shutdown", "/l"])

		elif "write a note" in query:
			speak("What do you want me to write?")
			note = takeCommand()
			file = open('friday.txt', 'w')
			speak("Should i even include date and time?")
			snfm = takeCommand()
			if 'yes' in snfm or 'sure' in snfm:
				strTime = datetime.datetime.now().strftime("%H:%M:%S")
				file.write(strTime)
				file.write(" :- ")
				file.write(note)
			else:
				file.write(note)
		
		elif "show note" in query:
			speak("Showing Notes")
			with open("friday.txt", "r") as file:
				notes = file.read()
			print(notes)
			speak(notes)
					
		elif "friday" in query:
			wishMe()
			speak("Friday at your service human!")
			speak(assname)

		elif "wikipedia" in query:
			webbrowser.open("wikipedia.com")

		elif "Good Morning" in query:
			speak("A warm" +query)
			speak("How are you Human?")
			speak(assname)

		elif "will you be my girlfriend" in query or "will you be my boyfriend" in query or "will you be my bf" in query or "will you be my gf" in query: 
			speak("Sorry, I am already taken!")

		elif "how are you" in query:
			speak("I'm fine, human")

		elif "i love you" in query:
			speak("Want me to call the mental helpline?")

		elif "what is" in query or "who is" in query:
			query_wolframalpha(query)
