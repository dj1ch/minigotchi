# Turns on light to indicate it is on
import time
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
    print("Starting the server...")
    controller.start_server()
    print("done")
    print("The server should be at 127.0.0.1:8666")
    print(" ")

def get_and_post(max_duration_seconds)
    start_time = time.time()
    print("\fSending requests...")
    controller.send_requests()
    print("done.")
    print(" ")
    # check for time
    elapsed_time = time.time() - start_time
        if elapsed_time >= max_duration_seconds:
            break

max_duration = 60
my_function(max_duration)

# run the script
setup()
get_and_post() # set how long these packets should be sent here
