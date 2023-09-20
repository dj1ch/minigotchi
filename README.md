## Not ready to be used yet(do not clone and install yet)
See the _To do_ section
# minigotchi
###
### an even smaller pwnagotchi.
###
Overall, this project started as a thought. I wanted to try to install pwnagotchi on a Raspbery pi pico. Obviously, due to hardware constraints, this will not be as possible as you may think. Well, I have a solution. One thing the pico can do is have a wifi adapter connected to it. It can have many attatchments. Some of these adapters can enter monitor mode which is crucial for this project. If I just so happen to make it find networks, randomly associate to the closest one, and broadcast the "pwnagotchi packet", other pwnagotchis will detect it, hence the name minigotchi. It will act as a friend to the pwnagotchi. Think of it as a pet. If possible, I can try to make it pwn some networks.
###
### Prerequisites
- Raspberry pi pico (not the pico w)
- Esp8266 wifi adapter (or any supported by nodemcu)
- Breadboard
- A supported IDE
- A seperate computer capable of transferring files
- Internet connection
###

### How it operates/works
###
The minigotchi relies on the IDE connected to the pico. The pico will attempt to associate to a local access point, then send packets through that network on a random channel. The logs will be put through the IDE(preferably thonny in this case). NodeMCU firmware is needed to do this. However, in order for this to work, we will need picolua, a project that will help the firmware work correctly.
###
### To do
- Get hardware(esp8266, breadboard, more hardware space)
- Install luapico(a dependancy to nodemcu)
- Install NodeMCU
- Use the firmware to start everything
- Make repo public
###
