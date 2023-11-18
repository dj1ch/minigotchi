# Install guide
Here you can find out how to install minigotchi on the esp8266, or the raspberry pi pico. The esp8266 microcontroller has more support than the pico at the moment. They can be both built using arduino, ~~but you can build manually with the pico, assuming that you have all the libraries from the arduino esp8266 repo. Again, building with Arduino IDE is an option for the raspberry pi pico.~~ The support for this has been dropped, as this is not maintained nor tested by me. 
#### Building using Arduino IDE
I believe that you can also use the raspberry pi pico and run arduino .ino files. It would be much easier to do this then to build it yourself manually.
- Download the latest release [here](https://github.com/Pwnagotchi-Unofficial/minigotchi/releases)
- Go to your arduino folder (on linux, it should be `~/Arduino`)
- Create a folder called `minigotchi`
- Copy and paste all the files from the release into the folder `~/Arduino/minigotchi`
- Open the arduino IDE in that directory (right clicking the .ino file and opening it with the IDE)
- It should prompt you to create a new folder called minigotchi, listen to what it tells you to do.
- Add additional .h files if needed, make sure you install all the dependancies
- From here you can select the board, and then you can run it on the board. Make sure you add the esp8266 libraries, which are on the arduino repo I put [at the bottom here](README.md)
- You first verify, then upload the files to the board using the IDE. The IDE should let you know when it's done programming the board. You can then go to the serial monitor and the board should be giving output, in this case it is showing the packets being sent.
- Note that the pwnagotchi will not notice these packets, yet.
####
