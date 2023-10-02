# Turns on light to indicate it is on
from controller import Controller
from machine import Pin, Timer
led = Pin(25, Pin.OUT)
led.toggle()

# startup
print("-- Minigotchi by dj1ch --")
print("\nYou can turn off the light on the pico by running 'led.toggle()' in the terminal here.")

# setup
def setup()
    controller = Controller()
    controller.__init__()
    print("Starting in client mode...")
    controller.esp_as_client()
    print("done.")
    print(" ")

def server()
    print("Starting the server...")
    controller.start_server()
    print("done")
    print(" ")

def get_and_post()
    print("\fSending requests...")
    controller.send_requests()
    print("done.")
    print(" ")

# run the script
setup()
get_and_post()
