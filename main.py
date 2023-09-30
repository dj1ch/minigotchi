# Turns on light to indicate it is on
from controller import Controller
from machine import Pin, Timer
led = Pin(25, Pin.OUT)
led.toggle()

# startup
print("-- Minigotchi by dj1ch --")
print("\nYou can turn off the light on the pico by running 'led.toggle()' in the terminal here.")
