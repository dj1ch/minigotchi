-- load modules
wifi = require("wifi")
local message = "123"

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
    local ssid = "SSID of your network you want to associate to"
    wifi.packet.frame_type = 0x00
    associationPacket.frame_body = ssid
    associationPacket:send()
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
