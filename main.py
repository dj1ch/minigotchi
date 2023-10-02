# setup the server, associating, etc
def setup():
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

# sending get and post requests to specified ip
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

# run the script
setup()
max_duration = 60
get_and_post(max_duration)
