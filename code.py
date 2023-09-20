# As of right now, this is just a template for the code. Waiting for actual hardware so I can test.
# Imports here
import board
import network
import socket

# Start
print("Minigotchi by dj1ch")
# Starts wifi card and monitor mode
try:
    wlan = network.WLAN(network.STA_IF)
    wlan.active(False)
    wlan.config(mode=network.WLAN.MODE_MONITOR)
    wlan.active(True)
    print("Wi-Fi interface in monitor mode")
except Exception as e:
    print("Error:", e)

# Associates and sends packets along network

#
