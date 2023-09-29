-- load modules
wifi = require("wifi")

-- variables (set them)
local message = "123" -- this should be the pwnagotchi information
local ssid = "SSID of network" -- set the SSID of the network
local mac = "mac of the local pwnagotchi"

-- functions
function start()
    wifi.monitor.start(13, 0x80, function(packet)
        print('Beacon: ' .. packet.bssid_hex .. " '" .. packet[0] .. "' ch " .. packet[3]:byte(1))
    end)
    print("Wi-Fi monitoring started")
end

function channel(channelNumber)
    wifi.monitor.channel(channelNumber)
    print("Switched to channel " .. channelNumber)
end

function associate()
    wifi.packet.frame_type = 0x00
    wifi.packet.frame_body = ssid
    wifi.packet:send()
    print("Associated to: " .. ssid)
end

function pwnagotchiPacket()
    wifi.packet.frame_type = 0x08
    wifi.packet.dstmac_hex = mac
    wifi.packet.frame_body = message
    wifi.packet:send()
    print("Broadcasted: " .. message)
end

-- execute functions
start()
channel(6)
associate()
pwnagotchiPacket()
