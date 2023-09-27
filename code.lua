-- load modules
wifi = require("wifi")
local message = "123"

-- functions
function wifi.monitor.start()
    print("Wi-Fi monitoring started")
end

function wifi.monitor.channel(channelNumber)
    print("Switched to channel " .. channelNumber)
end

function sendBroadcastPacket()
    local packet = wifi.packet()
    packet.frame_type = 0x00
    packet.frame_body = message
    packet:broadcast()
    print("Broadcasted: " .. message)
end

-- execute functions
wifi.monitor.start()
wifi.monitor.channel(6)
sendBroadcastPacket()
