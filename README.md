## Support for both the raspberry pi pico and the esp8266
#### See the _To do_ section
# minigotchi
###
### an even smaller pwnagotchi.
###
### Note that this project is undergoing a major rewrite in the code and the structure of everything(see development branch)
###
#### Intro
Overall, this project started as a thought. A pwnagotchi on an even smaller board, in this case an esp8266. Crazy, right? Anyway, this project may make it a little bit more possible. Instead of pwning networks, it will be a friend to a local pwnagotchi(Most likely one of yours), along with deauthing random networks if the esp8266 is in the vicinity of any.
####
#### Install?
The install guide is [here](INSTALL.md), now that I have put out releases.
####
#### Languages
- C
- C++
- Python
- Go (maybe)
####
#### Prerequisites
- An IDE (most likely the arduino ide or thonny on the raspberry pi pico)
- Hardware(esp8266 microcontroller or raspberry pi pico with esp8266 wifi module)
- Patience (a lot of it)
#### How it operates/works
The minigotchi relies on the IDE/serial shell for communication through logs. It will send raw frames to associate to an access point, and will advertise itself on a network like a pwnagotchi, or perhaps interact with the local pwngrid server that the pwnagotchi is running.
#### Something that you should know
We have dropped support for the pico, as it turns out, the esp8266 is a microcontroller itself(Though I can make progress for this in the future. For now, I am sticking with the esp8266).
####
#### Hardware
- Hardware is here(the esp8266):

https://www.amazon.com/QCCAN-Internet-ESP8266-Compatible-MicroPython/dp
####
#### To do
~~- Get hardware(esp8266/esp32, maybe breadboard?)~~
- Learn C/C++ basics(I still don't really get it 100%)
- **TEST!!!!!!!!!**
~~- Work on implementing a deauth system, so it could randomly deauth local aps(this will be added around version 0.3.0 alpha hopefully)~~
- Restructure code and make important changes to this project
####
#### Contributing
Contributions to the code itself are going to be accepted! Small, and potentially useless pull requests will be ignored. I don't want to waste your time or my time on something as insignificant as adding a space, or putting a link to something I don't need. See more [here](CONTRIBUTING.md). Thank you!
####
#### Last but not least... Some thank yous!
##### Thanks to these projects for making this a little bit more possible for me:

- Pico-sdk, allowing C/C++ to be compiled and ran on the raspberry pi pico

https://github.com/raspberrypi/pico-sdk

- Arduino, mostly just the repository containing libraries for the project so I can build this code.

https://github.com/esp8266/Arduino

- 80211Raw, which allows raw frames to be received and sent through an esp8266 or esp32 using C and C++ in an interesting and useful POC

https://github.com/tht/80211Raw

- esp8266 deauther, the POC that shows that an esp8266 can deauth and attack local access points.

https://github.com/SpacehuhnTech/esp8266_deauther/

- esp32 marauder, similar to the esp8266 deauther, although packed with much more features along with support for the flipper zero

https://github.com/justcallmekoko/ESP32Marauder
