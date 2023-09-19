# minigotchi
###
### an even smaller pwnagotchi.
###
Overall, this project started as a thought. I wanted to try to install pwnagotchi on a pico. Obviously, due to hardware constraints, this will not be as possible as you may think. Well, I have a solution. One thing the pico can do is have a wifi adapter connected to it. It can have many attatchments. Some of these adapters can enter monitor mode which is crucial for this project. If I just so happen to make it find networks, randomly associate to the closest one, and broadcast the "pwnagotchi packet", other pwnagotchis will detect it, hence the name minigotchi. It will act as a friend to the pwnagotchi. Think of it as a pet. If possible, I can try to make it pwn some networks.
###
### Prerequisites
- Raspberry pi pico (not the pico w)
- A supported IDE
- A seperate computer capable of transfering files
- Internet connection
###

### How it operates
###
The minigotchi relies on the IDE connected to the pico. The pico will attempt to associate to a local access point, then send packets through that network on a random channel. The logs will be put through the IDE(preferably thonny in this case). 
###
