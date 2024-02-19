# minigotchi
<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/Pwnagotchi-Unofficial/minigotchi"></img>
<img alt="GitHub license" src="https://img.shields.io/github/license/Pwnagotchi-Unofficial/minigotchi"></img>
<a href="https://github.com/Pwnagotchi-Unofficial/minigotchi/graphs/contributors"><img alt="GitHub contributors" src="https://img.shields.io/github/contributors/Pwnagotchi-Unofficial/minigotchi"></a>
<img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/Pwnagotchi-Unofficial/minigotchi"></img>
<img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/Pwnagotchi-Unofficial/minigotchi"></img>

###
### an even smaller Pwnagotchi.
###

<img src="images/Desktop Screenshot 2024.02.17 - 18.10.08.12.png"></img>

### Intro

This is a project meant to showcase the use of raw WiFi frames and deauthentication in a *Pwnagotchi-like* matter with an ESP8266. It can detect pwnagotchis, send "advertisements" for another Pwnagotchi to detect, along with deauthenticating devices connected to an AP. Originally meant to just befriend a local Pwnagotchi, I plan on improving it and extending its purpose.

###
### Install guide

See the [install guide](INSTALL.md).

###
### Languages
The code is written in these languages.

- *C*
- *C++*

Note that the C++ in arduino is slightly modified from what I have heard. See [this reddit question](https://www.reddit.com/r/arduino/comments/x46sml/is_arduino_programming_language_c/) for more about this. Arduino probably has tweaked it to be more beginner friendly, although still being [low-level](https://en.wikipedia.org/wiki/Low-level_programming_language). The *Pwnagotchi* is a complex machine written in *Python* and *Go*, so code from it is **not** compatible with mine. 

###
### Prerequisites

- An IDE (most likely the arduino ide)
- Hardware(D1 Mini ESP8266)
- A reliable and appropriate power source and supply for the hardware
- Patience (a lot of it)

###
### How it operates/works

See the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/How-the-Minigotchi-works).

###
### Hardware

See the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/Hardware).

###
### To do
See this [project](https://github.com/orgs/Pwnagotchi-Unofficial/projects/4).
###
### Contributing

Contributions to the code itself are going to be accepted! Small, and potentially useless pull requests will be ignored. I don't want to waste your time or my time on something as insignificant as adding a space, or putting a link to something I don't need. See more [here](CONTRIBUTING.md). Thank you!

###
### FAQ

See the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/FAQ).

### Communication

See the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/Communication#communication).

###
### Last but not least... Some thank you's!

**Thanks to these projects for making this a little bit more possible for me, I have used libraries/code from these projects and implemented it here:**

- Arduino, mostly just the repository containing libraries for the project so I can build this code. There were many libraries that were used in this core repository. 

https://github.com/esp8266/Arduino

- 80211Raw, which allows raw frames to be received and sent through an ESP8266 or ESP32 using C and C++ in an interesting and useful POC. This is used to send the *Pwnagotchi-like* packets/frames. Most of the Minigotchi is built around this library. 

https://github.com/tht/80211Raw

- ESP8266 deauther, the POC that shows that an ESP8266 can deauth and attack local access points. Deauthing is based off of this. 

https://github.com/SpacehuhnTech/esp8266_deauther/

- ESP32 Marauder, similar to the ESP8266 deauther, although packed with much more features along with support for the flipper zero ESP32. It has wardriving capabilities, BLE, and deauthing features available. 

https://github.com/justcallmekoko/ESP32Marauder

- ESPAsyncWebServer, the library that allows a Web UI on the ESP8266, along with an AP. 

https://github.com/lacamera/ESPAsyncWebServer

- Milkydeveloper, idk what he did but he wanted to be here ig so...(this is totally not an advertisement)

https://github.com/MilkyDeveloper
