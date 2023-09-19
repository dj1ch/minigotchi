# to do: add nodemcu firmware, add adafruit stuff 
import board
import busio
from digitalio import DigitalInOut
import adafruit_requests as requests
import adafruit_esp32spi.adafruit_esp32spi_socket as socket
from adafruit_esp32spi import adafruit_esp32spi
from secrets import secrets
# this is based on the guide https://www.tomshardware.com/how-to/get-wi-fi-internet-on-raspberry-pi-pico
print("Minigotchi by dj1ch")
# esp objects
esp32_cs = DigitalInOut(board.GP13)
esp32_ready = DigitalInOut(board.GP14)
esp32_reset = DigitalInOut(board.GP15)
# spi object
spi = busio.SPI(board.GP10, board.GP11, board.GP12)
#
