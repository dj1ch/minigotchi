# minigotchi
<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/Pwnagotchi-Unofficial/minigotchi"></img>
<img alt="GitHub license" src="https://img.shields.io/github/license/Pwnagotchi-Unofficial/minigotchi"></img>
<a href="https://github.com/Pwnagotchi-Unofficial/minigotchi/graphs/contributors"><img alt="GitHub contributors" src="https://img.shields.io/github/contributors/Pwnagotchi-Unofficial/minigotchi"></a>
<img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/Pwnagotchi-Unofficial/minigotchi"></img>
<img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/Pwnagotchi-Unofficial/minigotchi"></img>

###
### an even smaller pwnagotchi.
###

#### Intro
Overall, this project started as a thought. A pwnagotchi on an even smaller board, in this case an esp8266. Crazy, right? Instead of grabbing handshakes, it will be a friend to a local pwnagotchi(most likely one of yours), along with deauthing random networks if the esp8266 is in the vicinity of any.
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
#### FAQ
I'll add some more questions that I can answer here, but here are some I can answer.

**Why is this in a zip file, not a bin file?**

Long story short you will not be able to customize it after first boot. It's gonna stay like this forever, so any changes you make will be permanent. Plus, you're gonna need to whitelist your home network, channels it will change to, etc.

**Will there be flipper zero support?**

Maybe. I will need to test on the esp32, which at the moment I currently do not have. 

**How long until the next release?**

Probably a couple weeks at the most. I gotta finish up/fix a bunch of things, but feel free to check out this [project](https://github.com/orgs/Pwnagotchi-Unofficial/projects/4) to see my progress towards our goals.

**Why the esp8266, why not the esp32?**

Do one thing and do it right. I don't want to spend too much on a development board, and the *LOLIN D1 Mini* is small and effective, not having to pull much power, all at a cheaper price. I don't want it to be too big, the *minigotchi* is called *mini* for a reason. There aren't a lot of projects out there for this board anyway, the esp32 has a lot of different projects surrounding that board, which means this will project won't be as relevant to anyone. Do we even need bluetooth, when we have raw WiFi frames being sent to access points? I guess not right? 

**What's your motivation behind this project?**

I'm sure I have said this, but there aren't a lot of pwnagotchis. A major feature with them is having the ability to make friends. I don't want my pwnagotchi to sit bored at home while I am at school, so it would be nice to have something keep it company. 

**Help, there's an error! How do I fix it?**

Get the output and paste it in an [issue](https://github.com/Pwnagotchi-Unofficial/minigotchi/issues), usually the output can tell you what the problem is. Be sure to ask [google](https://google.com) beforehand, I don't want clueless people asking dumb questions which can be solved with a google search. 

**The board stopped working! The light will not turn on with my current power supply!**

Congragulations, you have burnt the board! Most likely you gave a really high voltage to the board and that caused it to fail. I'm pretty sure it is supposed to run at 3.3 volts. Feel free to get more info with the documention [here](https://www.wemos.cc/en/latest/d1/d1_mini.html).

**Is the project really working right now?**

Haha... no. Frames aren't valid(at least by [IEEE 802.11 standards](https://en.wikipedia.org/wiki/IEEE_802.11)), and apparently were not being detected by my pwnagotchi. I have no idea if things are actually fully working, although the board is programmed successfully. It's a really long story, but I hope to fix everything by the next release. 

**Is this project dead?**

Nope. Never will be unless I publicly declare it and I let the organization take it over. Most likely my friend @rai68 will take it over. 

**How can I support your work?**

I have an Open Collective [account](https://opencollective.com/minigotchi), if that helps.

#### Wanna ask me questions? 

You can contact me through:

- Discord. My username is *dj1ch*. I'm online a lot and open to DM's at the moment.

- The Github discussion section of this [repository](https://github.com/Pwnagotchi-Unofficial/minigotchi/discussions/31)

- My [portfolio website](https://dj1ch.github.io), it has a messaging system that allows you to send messages to me. Just remember not to spam me. 

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
