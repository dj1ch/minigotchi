# Turns on light to indicate it is on
import board
from machine import Pin, Timer
led = Pin(25, Pin.OUT)
light=Light()

def blink(light)
    led.toggle()

# Startup
print("Minigotchi by dj1ch")
print("\nRun the script by typing 'lua <filename here>'.")
