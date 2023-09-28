-- load modules
wifi = require("wifi")
local message = "123"

-- functions
function wifi.monitor.start()
    wifi.monitor.start
    print("Wi-Fi monitoring started")
end

function wifi.monitor.channel(channelNumber)
    wifi.monitor.channel(channelNumber)
    print("Switched to channel " .. channelNumber)
end

function associate
-- put the stuff here
end

function pwnagotchipacket()
    local packet = wifi.packet()
    packet.frame_type = 0x08
    packet.frame_body = message
    packet:broadcast()
    print("Broadcasted: " .. message)
end

-- execute functions
wifi.monitor.start()
wifi.monitor.channel(6)
pwnagotchipacket()
