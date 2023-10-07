import machine
from config import Config
import utime
import sys


class ESP8266:
    def __init__(self, uart, debug=False):
        self.uart = uart

        self.state_pin = machine.Pin(Config.STATE_PIN_ID, machine.Pin.OUT)
        self.reset_pin = machine.Pin(Config.RESET_PIN_ID, machine.Pin.OUT)

        self.debug = debug

    def startup(self):
        self.state_pin.on()

        self.reset_pin.off()

        utime.sleep(2)
        self.reset_pin.on()

    def check_module(self):
        status, data = self.send_cmd("AT", "ready")

        return status

    def disconnect_from_network(self):
        self.send_cmd("AT+CWQAP")

    def connect_to_network(self, ssid, password):
        self.disconnect_from_network()

        self.send_cmd(f'AT+CWJAP="{ssid}","{password}"')
        self.send_cmd("AT+CWMODE=1")
        self.send_cmd("AT+CIPMODE=0")

    def init_hotspot(self, ssid, password):
        self.disconnect_from_network()

        self.send_cmd(f'AT+CWSAP="{ssid}","{password}",1,4')

        self.send_cmd("AT+CWMODE=2")
        self.send_cmd("AT+CIPMODE=0")

    def start_server(self, port):
        self.send_cmd("AT+CIPMUX=1")

        status, _ = self.send_cmd(f"AT+CIPSERVER=1,{port}")

        return status

    def get_connected_devices(self):
        self.send_cmd("AT+CWLIF")

    def get_address_as_client(self):
        status, data = self.send_cmd("AT+CIFSR")

        return self.parse_cmd_data(data)

    def get_address_as_host(self):
        status, data = self.send_cmd("AT+CIPAP?")

        return self.parse_cmd_data(data)

    def is_connected_to_wifi(self):
        connected = False

        status, data = self.send_cmd("AT+CIPSTATUS")

        if status:
            parsed_cmd_data = self.parse_cmd_data(data)

            if "STATUS:2" in parsed_cmd_data:
                connected = True

        return connected

    def check_connection_with_target(self, target_ip):
        status, data = self.send_cmd(f'AT+PING="{target_ip}"')

        return status

    def start_connection_with_target(self, target_ip, target_port):
        self.send_cmd(f'AT+CIPSTART="TCP","{target_ip}",{target_port}')

    def close_connection(self):
        self.send_cmd("AT+CIPCLOSE")

    def send_post(self, payload, headers, target_ip, destination_path, target_port):
        response_data = ""

        self.start_connection_with_target(target_ip, target_port)

        if self.check_connection_with_target(target_ip):
            request_header = [
                f"POST {destination_path} HTTP/1.1",
                f"Host: {target_ip}",
                "Content-Type: application/json",
                f"Content-Length: {str(len(payload))}"
            ]

            for header_key in headers:
                request_header.append(f"{header_key}: {headers.get(header_key)}")

            request_header = "\r\n".join(request_header)
            request_body = f"{payload}\r\n\r\n"

            request_data = f"{request_header}\r\n\r\n{request_body}"

            self.send_cmd(f"AT+CIPSEND={str(len(request_data))}", ">")
            status, data = self.send_cmd(request_data, "CLOSED")

            response_data = self.parse_request(data)

        else:
            self.close_connection()

        return response_data

    def send_get(self, target_ip, destination_path, target_port):
        response_data = ""

        self.start_connection_with_target(target_ip, target_port)

        if self.check_connection_with_target(target_ip):
            request_data = f"GET {destination_path} HTTP/1.1\r\nHost: {target_ip}\r\n\r\n"

            self.send_cmd(f"AT+CIPSEND={len(request_data)}", ">")
            status, data = self.send_cmd(request_data, "CLOSED")

            response_data = self.parse_request(data)

        else:
            self.close_connection()

        return response_data

    def parse_cmd_data(self, cmd_data):
        cmd_data = cmd_data.replace("\r", "").replace("\"", "").split("\n")
        cleared_cmd_data = " ".join([row for row in cmd_data if row != ""])

        return cleared_cmd_data

    def parse_request(self, request_data):
        request_content = ""
        splitted_request = request_data.split("+IPD")

        if len(splitted_request) > 0:
            request_content = splitted_request[-1].replace("CLOSED\r\n", "")
            request_content = request_content.split(":")

            request_content.pop(0)

            request_content = ":".join(request_content)

        return request_content

    def parse_server_request(self, request_data):
        request_content = ""
        splitted_request = request_data.split(":")

        if len(splitted_request) > 0:
            splitted_request.pop(0)

            request_content = ":".join(splitted_request)

        return request_content

    def send_cmd(self, cmd, ack="OK", timeout=5000):
        status = False
        output_data = ""
        current_time = utime.ticks_ms()

        self.uart.write(f"{cmd}\r\n")

        while (utime.ticks_ms() - current_time) < timeout:
            uart_row = self.uart.read()

            if uart_row is not None:
                try:
                    row_data = uart_row.decode()
                    output_data += row_data

                    self.print_debug(uart_row)

                    if ack in row_data:
                        status = True
                        break

                except Exception as e:
                    self.print_debug(str(e), exception=e)

        return status, output_data

    def server_mainloop(self, requests_handler):
        while True:
            uart_row = self.uart.read()

            if uart_row is not None:
                try:
                    decoded_row_data = self.parse_server_request(uart_row.decode())
                    self.print_debug(decoded_row_data)

                    requests_handler(decoded_row_data)

                except Exception as e:
                    self.print_debug(str(e), exception=e)

    def print_debug(self, message, exception=None):
        if self.debug:
            print(message)

            if exception:
                sys.print_exception(exception)
