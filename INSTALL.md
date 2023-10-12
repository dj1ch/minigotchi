#### DO NOT INSTALL ANY OF THE FILES ONTO THE PICO!
Mostly just figuring out C++ and C, that allows the raw frames to be sent. Long story short I may need to learn C++ and C, and at least just figuring out variables. The branch called "saved" is an archive of the work in python, using the esp8266 wrapper, although it will not work, since I have not made the progress in defining the headers being sent to the pwnagotchi.

#### Manually building it
clone the github repo, then cd into it. if you are building and testing, i recommend making a seperate directory for testing and then another with the actual github repo with your changes in it. make sure you get all the dependancies on your system. this includes the arduino esp8266 and esp32 libraries.

- `cd minigotchi` (or whatever you named the testing directory)
- `mkdir build`
- `cmake ..`
- `make`

from here it should generate the stuff needed for the esp8266, and the files should be in the build folder. If you can't find it you can always run `sudo find / -name raw*.uf2`, which will find the files that you will need(at least the .uf2 files.).
####

That's how you can do it manually, here's you do it in a more easier manner.
#### Building using Arduino IDE
I believe that you can also use the raspberry pi pico and run arduino .ino files. It would be much easier to do this then to build it yourself manually. 
- Clone repo, and open it up in the file manager
- Go to your arduino folder (on linux, it should be `~/Arduino`)
- Create a folder called `minigotchi`, then in that folder create another folder called `minigotchi`
- Copy and paste the .h and .ino file from this repo the folder `~/Arduino/minigotchi/minigotchi`
- From here you can select the board, and then you can run it on the board. Make sure you add the esp8266 libraries, which are on the arduino repo I put [at the bottom here](README.md)
####
#### Note
~~We dropped support for the pico, but there is a project out there for the pico w.~~ I mean, there still is potential support for the pico, if you are willing to build the files correctly. This has to be done manually, and I am sure there are resources available here to do so. When in doubt, check the raspberry pi documentation.
