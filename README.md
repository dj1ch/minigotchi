# minigotchi
<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/Pwnagotchi-Unofficial/minigotchi"></img>
<img alt="GitHub license" src="https://img.shields.io/github/license/Pwnagotchi-Unofficial/minigotchi"></img>
<a href="https://github.com/Pwnagotchi-Unofficial/minigotchi/graphs/contributors"><img alt="GitHub contributors" src="https://img.shields.io/github/contributors/Pwnagotchi-Unofficial/minigotchi"></a>
<img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/Pwnagotchi-Unofficial/minigotchi"></img>
<img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/Pwnagotchi-Unofficial/minigotchi"></img>

###
### an even smaller pwnagotchi.
###

### Intro

Overall, this project started as a thought. A pwnagotchi on an even smaller board, in this case an ESP8266. Crazy, right? Instead of grabbing handshakes, it will be a friend to a local pwnagotchi(most likely one of yours), along with deauthing random networks if the ESP8266 is in the vicinity of any.

###
### Install guide

The install guide is [here](INSTALL.md), now that I have put out releases.

###
### Languages
The code is written in these languages.

- C
- C++

Note that the C++ in arduino is slightly modified from what I have heard. See [this reddit question](https://www.reddit.com/r/arduino/comments/x46sml/is_arduino_programming_language_c/) for more about this. Arduino probably has tweaked it to be more beginner friendly, although still being [low-level](https://en.wikipedia.org/wiki/Low-level_programming_language).

###
### Some other ones...

- Python
- Go

Although nothing will be coded in these langauges(most likely) on this project, it will still be good to know these languages as they will help us understand how a pwnagotchi works. It's a complicated machine, and takes a while to fully understand considering how complex it can be.

###
### Prerequisites
- An IDE (most likely the arduino ide)
- Hardware(D1 Mini ESP8266)
- A reliable and appropriate power source and supply for the hardware
- Patience (a lot of it)
### How it operates/works

The minigotchi relies on the IDE/serial shell for communication through logs. It will send raw frames to associate to an access point, and will advertise itself on a network like a pwnagotchi, or perhaps interact with the local pwngrid server that the pwnagotchi is running.

###
### Hardware

See the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/Hardware).

###
### To do
See this [project](https://github.com/orgs/Pwnagotchi-Unofficial/projects/4)
###
### Contributing

Contributions to the code itself are going to be accepted! Small, and potentially useless pull requests will be ignored. I don't want to waste your time or my time on something as insignificant as adding a space, or putting a link to something I don't need. See more [here](CONTRIBUTING.md). Thank you!

###
### FAQ

See the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/FAQ).

### Communication

See the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/Communication#communication).

###
### Last but not least... Some thank yous!
**Thanks to these projects for making this a little bit more possible for me, I have used libraries/code from these projects and implemented it here:**

- Arduino, mostly just the repository containing libraries for the project so I can build this code. There were many libraries that were used in this core repository. 

https://github.com/esp8266/Arduino

- 80211Raw, which allows raw frames to be received and sent through an ESP8266 or ESP32 using C and C++ in an interesting and useful POC. This is used to send the *Pwnagotchi*-like packets/frames.

https://github.com/tht/80211Raw

- ESP8266 deauther, the POC that shows that an ESP8266 can deauth and attack local access points. Deauthing is based off of this. 

https://github.com/SpacehuhnTech/esp8266_deauther/

- ESP32 Marauder, similar to the ESP8266 deauther, although packed with much more features along with support for the flipper zero ESP32. It has wardriving capabilities, BLE, and deauthing features available. 

https://github.com/justcallmekoko/ESP32Marauder

- ESPAsyncWebServer, the library that allows a Web UI on the ESP8266, along with an AP. 

https://github.com/lacamera/ESPAsyncWebServer

- Milkydeveloper, idk what he did but he wanted to be here ig so...

https://github.com/MilkyDeveloper
