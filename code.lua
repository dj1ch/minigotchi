-- load modules
wifi = require("wifi")
-- variables(set them)
local message = "123" -- this should be the pwnagotchi information
local ssid = "SSID of network" -- set the ssid of the network

-- functions
function start()
    wifi.monitor.start
    print("Wi-Fi monitoring started")
end

function channel(channelNumber)
    wifi.monitor.channel(channelNumber)
    print("Switched to channel " .. channelNumber)
end

function associate()
    wifi.packet.frame_type = 0x00
    associationPacket.frame_body = ssid
    associationPacket:send()
    print("Associated to: " .. ssid)
end

function pwnagotchipacket()
    wifi.packet.frame_type = 0x08
    wifi.packet.frame_body = message
    wifi.packet:broadcast()
    print("Broadcasted: " .. message)
end

-- execute functions
start()
channel(6)
associate()
pwnagotchipacket()
