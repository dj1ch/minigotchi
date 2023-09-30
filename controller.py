import machine
from config import Config
from esp8266 import ESP8266


class Controller:
    def __init__(self):
        self.uart = machine.UART(Config.UART_ID, Config.UART_BAUDRATE)
        self.esp = ESP8266(self.uart, debug=True)

        self.wifi_ssid = "" # Must be set
        self.wifi_password = "" # Must be set

        self.hotspot_ssid = "test_hotspot"
        self.hotspot_password = "hotspot1234"

    def esp_as_client(self):
        print(f"Connecting to network: {self.wifi_ssid}")
        self.esp.connect_to_network(self.wifi_ssid, self.wifi_password)

        if self.esp.is_connected_to_wifi():
            print(f"Address data: {self.esp.get_address_as_client()}")

    def esp_as_host(self):
        print(f"Creating network: {self.hotspot_ssid}")
        self.esp.init_hotspot(self.hotspot_ssid, self.hotspot_password)

        address_data = self.esp.get_address_as_host()

        if len(address_data) > 0:
            print(f"Address data: {address_data}")

    def server_requests_handler(self, request_data):
        pass

    def start_server(self):
        print("Starting server at port 8080")

        if self.esp.start_server(8080):
            print("Server running...")

            self.esp.server_mainloop(self.server_requests_handler)

    def send_requests(self):
        get_data = self.esp.send_get("192.168.1.105", "/get", 8080)
        post_data = self.esp.send_post(str({'key': 'value'}), {}, "192.168.1.105", "/post", 8080)

        print(f"POST data: {post_data}")
        print(f"GET data: {get_data}")

    def mainloop(self):
        print("Starting ESP8266...")
        self.esp.startup()

        if self.esp.check_module():
            print("ESP8266 running...")

            self.esp_as_client()
            # self.esp_as_host()

            # self.send_requests()
            self.start_server()
