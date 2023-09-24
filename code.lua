--Maybe some progress here??
function wifi.monitor.start()
    print("Wi-Fi monitoring started")
end

-- let's just assume that the pwnagotchi will start on channel 6 on bootup
--local channelNumber
--channelNumber = 6

function wifi.monitor.channel(channelNumber)
    print("Switched to channel " .. channelNumber)
end

-- executing the functions...
wifi.monitor.start()
wifi.monitor.channel(6)
