import machine
from config import Config
from esp8266 import ESP8266


class Controller:
    def __init__(self):
        self.uart = machine.UART(Config.UART_ID, Config.UART_BAUDRATE)
        self.esp = ESP8266(self.uart, debug=True)

        self.wifi_ssid = "" # Must be set
        self.wifi_password = "" # Must be set

    def esp_as_client(self):
        print(f"Connecting to network: {self.wifi_ssid}")
        self.esp.connect_to_network(self.wifi_ssid, self.wifi_password)

        if self.esp.is_connected_to_wifi():
            print(f"Address data: {self.esp.get_address_as_client()}")

    def send_requests(self):
        get_data = self.esp.send_get("127.0.0.1", "/get", 8666) # 127.0.0.1 is localhost, so idk 
        post_data = self.esp.send_post(str({'key': 'value'}), {}, "127.0.0.1", "/post", 8666)

        print(f"POST data: {post_data}")
        print(f"GET data: {get_data}")
