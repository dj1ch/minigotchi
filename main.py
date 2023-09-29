# Turns on light to indicate it is on
from machine import UART
import machine
import _thread
import time
import utime
from machine import UART
from machine import Pin, Timer
led = Pin(25, Pin.OUT)
led.toggle()

# startup
print("-- Minigotchi by dj1ch --")
print("\nYou can turn off the light on the pico by running 'led.toggle()' in the terminal here.")


# Setup(s)
print("Setting up UART connections...")
uart = UART(1,115200) # set uart
print("done!")
print (" ")

def uartSerialRxMonitor():
    recv=""

    while True:
        recv=str(uart.read(1).decode("utf-8"))
        print(recv, end='')
        global recv_buf
        recv_buf=recv_buf+recv

print("Connecting to ESP8266...")
_thread.start_new_thread(uartSerialRxMonitor, ()) # start serial monitor as a thread
print("done!")
print(" ")

print("Connecting to wifi...")
print("  - Setting CWMODE to 1 station mode...")
uart.write('AT+CWMODE=1'+'\r\n')
time.sleep(2)
print('  - Joining Wifi ...')
uart.write('AT+CWJAP="wifissid","wifipassword"'+'\r\n') # set Wifi network SSID and password here
time.sleep(5)
print("done!")
print(" ")

# Sending packets
print("Sending packets...")

def run_duration(duration_ms, func)
    while utime.ticks_diff(utime.ticks_ms(), start_time) < duration_ms:
        func()

def send_packet

run_duration(5000, send_packet)

print("done!")
