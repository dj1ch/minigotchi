## Not ready to be used yet(do not clone and install yet)
#### See the _To do_ section
# minigotchi
###
### an even smaller pwnagotchi.
![Raspberry Pi](https://img.shields.io/badge/-RaspberryPi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)
![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54)
![Lua](https://img.shields.io/badge/lua-%232C2D72.svg?style=for-the-badge&logo=lua&logoColor=white)
<p align="center">
  <img width="200" height="200" src="/images/ra1ux5gj17u91.png">
</p>

###
#### Intro
Overall, this project started as a thought. A pwnagotchi on a raspberry pi pico. Crazy, right? Anyway, this project may make it a little bit more possible. Instead of pwning, it will be a friend to a local pwnagotchi(Most likely one of yours).
####
#### Install?
When I get everything fully working, the install guide will be ![here](INSTALL.md)
####
#### Prerequisites
- Raspberry pi pico (not the w, here is a project that does support it [https://github.com/BoxC4T/Pwnagotchi-Pico](url))
- Esp8266 wifi adapter (or any supported by the firmware)
- Breadboard
- A supported IDE(although you can get away with a serial shell, that will still work just fine. rshell, minicom, etc are good examples)
- A seperate computer capable of transferring files
- Internet connection
#### How it operates/works
The minigotchi relies on the IDE connected to the pico. The pico will attempt to associate to a local access point, then send packets through that network on a random channel. The logs will be put through the IDE(preferably thonny in this case).
#### Something that you should know
The thing is, the drive that mounts onto your computer is essentially a dummy drive. Everything you put into that drive gets copied to ram. Unfortunatly, the firmware that I need is not all going to fit on that drive alongside the circuitpython firmware. Finding a solution.
####
#### To do
- I should crop that image...
- Get hardware(esp8266, maybe breadboard?)
- Figure out the memory issue, get files to copy into pico
- LEARN LUA ASAP!!!!
#### Last but not least... Some thank yous!
##### Thanks to these projects for making this a little bit more possible for me:
- picolua, a project that allows lua to be run on a raspberry pi pico

https://github.com/kevinboone/luapico

- uf2, the creator of the uf2 files, but also allowing us to convert .bin files to .uf2

https://github.com/microsoft/uf2

- nodeMCU, the firmware which helps the esp card enter monitor mode and inject packets.

https://github.com/nodemcu/nodemcu-firmware
