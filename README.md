# minigotchi
###
### an even smaller pwnagotchi.
###
**Note that I have merged changes from development recently as to fix a recent pull request made to this repository. Use the latest [release](https://github.com/Pwnagotchi-Unofficial/minigotchi/releases/tag/v1.1.1), so there are no problems. I am working constantly to figure out everything and to push a new release soon.**
#### Intro
Overall, this project started as a thought. A pwnagotchi on an even smaller board, in this case an esp8266. Crazy, right? Anyway, this project may make it a little bit more possible. Instead of pwning networks, it will be a friend to a local pwnagotchi(Most likely one of yours), along with deauthing random networks if the esp8266 is in the vicinity of any.
####
#### Install?
The install guide is [here](INSTALL.md), now that I have put out releases.
####
#### Languages
The code is written in these languages.

- C
- C++

Note that the C++ in arduino is slightly modified from what I have heard. See [this reddit question](https://www.reddit.com/r/arduino/comments/x46sml/is_arduino_programming_language_c/) for more about this. Arduino probably has tweaked it to be more beginner friendly, although still being [low-level](https://en.wikipedia.org/wiki/Low-level_programming_language).
####
#### Some other ones...

- Python
- Go

Although nothing will be coded in these langauges(most likely) on this project, it will still be good to know these languages as they will help us understand how a pwnagotchi works. It's a complicated machine, and takes a while to fully understand considering how complex it can be.
####
#### Prerequisites
- An IDE (most likely the arduino ide)
- Hardware(esp8266 microcontroller)
- A reliable and appropriate power source and supply for the hardware
- Patience (a lot of it)
#### How it operates/works
The minigotchi relies on the IDE/serial shell for communication through logs. It will send raw frames to associate to an access point, and will advertise itself on a network like a pwnagotchi, or perhaps interact with the local pwngrid server that the pwnagotchi is running.
#### Something that you should know
We have dropped support for the pico, as it turns out, the esp8266 is a microcontroller itself(Though I can make progress for this in the future. For now, I am sticking with the esp8266).
####
#### Hardware
- Hardware is here(the esp8266):

https://www.amazon.com/QCCAN-Internet-ESP8266-Compatible-MicroPython/dp

Keep in mind it comes with two of them. It's best to keep one extra in case a board gets burnt out and/or lost. I don't think I can find a singular one. I believe this is a clone, so be sure to set this as the clone in the Arduino IDE board manager.
####
#### To do
See this [project](https://github.com/orgs/Pwnagotchi-Unofficial/projects/4)
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
