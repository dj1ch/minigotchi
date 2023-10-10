#### DO NOT INSTALL ANY OF THE FILES ONTO THE PICO!
Mostly just figuring out C++ and C, that allows the raw frames to be sent. Long story short I may need to learn C++ and C, and at least just figuring out variables. The branch called "saved" is an archive of the work in python, using the esp8266 wrapper, although it will not work, since I have not made the progress in defining the headers being sent to the pwnagotchi.
#### How I am testing
clone the github repo, then cd into it.

- `cd minigotchi`
- `mkdir build`
- `cmake ..`
- `make`

from here it should generate the stuff needed for the esp8266, and the files should be in the build folder. If you can't find it you can always run `sudo find / -name raw*.uf2`, which will find the files that you will need(at least the .uf2 files.)
####
#### Note
~~We dropped support for the pico, but there is a project out there for the pico w.~~ I mean, there still is potential support for the pico, if you are willing to build the files correctly. This has to be done manually, and I am sure there are resources available here to do so. When in doubt, check the raspberry pi documentation.
