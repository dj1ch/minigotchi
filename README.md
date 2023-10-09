## Potential support for the raspberry pi pico returning back soon
#### See the _To do_ section
# minigotchi
###
### an even smaller pwnagotchi.
###
#### Intro
Overall, this project started as a thought. A pwnagotchi on an even smaller board, in this case an esp8266. Crazy, right? Anyway, this project may make it a little bit more possible. Instead of pwning, it will be a friend to a local pwnagotchi(Most likely one of yours). Sadly as of right now, no faces, it might show one when broadcasting.
####
#### Install?
When I get everything fully working, the install guide will be ![here](INSTALL.md)
####
#### Prerequisites
-
#### How it operates/works
The minigotchi relies on the IDE/serial shell for communication through logs. It will send raw frames to associate to an access point, and will advertise itself on a network like a pwnagotchi.
#### Something that you should know
We have dropped support for the pico, as it turns out, the esp8266 is a microcontroller itself(Though I can make progress for this in the future. For now, I am sticking with the esp8266).
####
#### To do
- Get hardware(esp8266/esp32, maybe breadboard?)
Hardware is here:
https://www.amazon.com/QCCAN-Internet-ESP8266-Compatible-MicroPython/dp

- Learn C/C++ basics
#### Last but not least... Some thank yous!
##### Thanks to these projects for making this a little bit more possible for me:

- Arduino, mostly just the repository containing libraries for the project so I can build this code.

https://github.com/esp8266/Arduino

- 80211Raw, which allows raw frames to be received and sent through an esp8266 or esp32 using C and C++

https://github.com/tht/80211Raw
