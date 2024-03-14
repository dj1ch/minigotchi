# minigotchi
<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/Pwnagotchi-Unofficial/minigotchi"></img>
<img alt="GitHub license" src="https://img.shields.io/github/license/Pwnagotchi-Unofficial/minigotchi"></img>
<a href="https://github.com/Pwnagotchi-Unofficial/minigotchi/graphs/contributors"><img alt="GitHub contributors" src="https://img.shields.io/github/contributors/Pwnagotchi-Unofficial/minigotchi"></a>
<img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/Pwnagotchi-Unofficial/minigotchi"></img>
<img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/Pwnagotchi-Unofficial/minigotchi"></img>

###
### An even smaller Pwnagotchi.
###

<img src="images/Desktop Screenshot 2024.03.14 - 13.22.26.36.png"></img>

### Intro

This is a project meant to showcase the use of raw WiFi frames and deauthentication in a *Pwnagotchi-like* matter with an ESP8266. It can detect pwnagotchis, and send "advertisements" for another Pwnagotchi to detect, along with deauthenticating devices connected to an AP. Originally meant to just befriend a local Pwnagotchi, I plan to improve it and extend its purpose, merely with an ESP8266 with 4 MB of memory. 

###
### Install guide

See the [install guide](INSTALL.md).

###
### Languages
The code is written in these languages.

- *C*
- *C++*

Note that the *C++* in Arduino is slightly modified from what I have heard. See [this Reddit question](https://www.reddit.com/r/arduino/comments/x46sml/is_arduino_programming_language_c/) for more about this. Arduino probably has tweaked it to be more beginner-friendly, although still being [low-level](https://en.wikipedia.org/wiki/Low-level_programming_language). The *Pwnagotchi* is a complex machine written in *Python* and *Go*, so code from the Pwnagotchi Project is **not** compatible with mine.

###
### Prerequisites

- Arduino IDE
- A good understanding of coding/programming
- D1 Mini ESP8266 V4
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

See the Contributing Guidelines [here](CONTRIBUTING.md).

###
### FAQ

See the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/FAQ).

### Communication

See the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/Communication#communication).

###
### Last but not least... Some thank you's!

**If you believe your project wasn't properly credited, please let me know ASAP! Here's how you can [communicate](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/Communication#communication) with me.**

**Thanks to these projects for making this a little bit more possible for me, I have used libraries/code from these projects and implemented it here:**

- Arduino, mostly just the repository containing libraries for the project so I can build this code. Many libraries were used in this core repository. 

https://github.com/esp8266/Arduino

- 80211Raw allows raw frames to be received and sent through an ESP8266 or ESP32 using C and C++ in an interesting and useful POC. This is used to send the *Pwnagotchi-like* packets/frames. Most of the Minigotchi is built around this library. 

https://github.com/tht/80211Raw

- ESP8266 Deauther, the POC that shows that an ESP8266 can deauth and attack local access points. Deauthing is based on this project and is done in a very similar fashion. 

https://github.com/SpacehuhnTech/esp8266_deauther/

- ESP32 Marauder, similar to the ESP8266 Deauther, although packed with more features and support for Flipper Zero's ESP32 WiFi Card Module. It has wardriving capabilities, BLE, Pwnagotchi Scanning, and Deauthing available. This is a very cool project!

https://github.com/justcallmekoko/ESP32Marauder

- ESPAsyncWebServer, the library that allows a Web UI on the ESP8266, along with an AP. 

https://github.com/lacamera/ESPAsyncWebServer

- Milkydeveloper, idk what he did but he wanted to be here ig so...(this is totally not an advertisement)

https://github.com/MilkyDeveloper

**Made with ❤️ by [@dj1ch](https://github.com/dj1ch) and the rest of the Pwnagotchi-Unofficial team!**
