# Turns on light to indicate it is on
from machine import UART
import machine
import _thread
import time
from machine import UART
from machine import Pin, Timer
led = Pin(25, Pin.OUT)
led.toggle()

# Startup
print("Minigotchi by dj1ch")
print("\nYou can turn off the light on the pico by running 'led.toggle()' in the terminal here.")

uart = UART(1,115200)
print('-- UART Serial --')
print('>', end='')
