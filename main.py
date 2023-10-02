# imports here
import time
from controller import Controller

# functions
def setup():
    controller = Controller()
    print("Starting in client mode...")
    controller.esp_as_client()
    print("done.")
    print("Starting the server...")
    controller.start_server()
    print("done")
    print("The server should be at 127.0.0.1:8666")
    print(" ")

def get_and_post(max_duration_seconds):
    start_time = time.time()
    while True:
        elapsed_time = time.time() - start_time
        if elapsed_time >= max_duration_seconds:
            break
        print("\fSending requests...")
        controller.send_requests()
        print("done.")
        print(" ")

if __name__ == "__main__":
    # logger
    logging.basicConfig(level=logging.INFO)

    # controller instance
    controller = Controller()

    # turn on the light to indicate the script is running
    led = Pin(25, Pin.OUT)
    led.toggle()

    print("-- Minigotchi by dj1ch --")
    print("\nYou can turn off the light on the pico by running 'led.toggle()' in the terminal here.")

    # run the script
    setup()
    max_duration = 60 # this is the amount of time, make sure it is set. 
    get_and_post(max_duration)
