# Install guide

Here you can find out how to install minigotchi on the ESP8266, or the raspberry pi pico. The ESP8266 microcontroller has more support than the pico at the moment. They can be both built using arduino, ~~but you can build manually with the pico, assuming that you have all the libraries from the arduino ESP8266 repo. Again, building with Arduino IDE is an option for the raspberry pi pico.~~ The support for this has been dropped, as this is not maintained nor tested by me.

### Building using Arduino IDE/Web editor

I believe that you can also use the raspberry pi pico and run arduino `.ino` files. It would be much easier to do this then to build it yourself manually. However, I don't support the pico, so if you run into error you're on your own.

- Download the latest release [here](https://github.com/Pwnagotchi-Unofficial/minigotchi/releases).
- Unzip the file
- Navigate to the `minigotchi.ino` file in the unzipped folder, and open it up
- At the line that says:

```cpp
raw.init("fo:od:ba:be:fo:od", 1); // set the settings here, ("BSSID", channel)
```

Replace the `"fo:od:ba:be:fo:od"` with your actual BSSID(in the quotations), and the `1` with the channel you prefer(not in quotations). Note that the pwnagotchi will constantly switch channels so you should go check the channels in the pwnagotchi's web UI.
- There should also be a line that says:

```cpp
deauthAttack.addToWhitelist("fo:od:ba:be:fo:od"); // add your ssid(s) here
deauthAttack.addToWhitelist("fo:od:ba:be:fo:od");
```

Replace the `fo:od:ba:be:fo:od` with the ssid's you want whitelisted. You can add more in this manner by copying and pasting this into the setup function

```cpp
deauthAttack.addToWhitelist("YOUR_SSID_HERE_OR_BSSID_IDC_REALLY");
```

- Save and exit the file.
- Download the [Arduino IDE](https://https://www.arduino.cc/en/software), preferably the latest version.
- In the IDE, go to *File* > *Preferences*, then in the *Additional board manager URLS*, paste the link "https://arduino.esp8266.com/stable/package_esp8266com_index.json" to add the ESP8266 board(s) to the board manager
- Click *Ok* and plug the board into your computer. It should blink, make sure it is receiving the correct amount of voltage, too much will burn it(I learned that the hard way)
- Open up the minigotchi folder through the IDE by pushing *Ctrl O*, or by going to *File* > *Open*, then selecting the folder the .ino is in. If other tabs don't show up, along with minigotchi.ino, make sure to copy and paste the files into the same directory/folder as that lone .ino file. It won't be able to compile if that happens. 
- Install the following dependancies with the library manager: `ArduinoJson`, `ESPAsyncWebServer`, all with their dependancies(Please install all of them for them to work correctly).
- Select the board as `ESP8266 WEMOS(LOLIN) D1 mini Lite`, and select the port it is plugged into(if you haven't already, plug in the board)
- Click on the upload button(arrow pointing to the left). 

**Note: if you get any errors, let me know ASAP with a github issue, sending me a discord message, or perhaps even messaging me through my portfolio site.**

- You can click on the *Serial Monitor* button on the top bar to see the serial monitor output. Make sure the baud rate is `115200`.

- **Compile errors happen with the Web editor, so you can just use the Arduino IDE app. Make sure you install all the correct libraries, and the correct [boards](https://github.com/esp8266/Arduino). See this issue I made [here](https://github.com/Pwnagotchi-Unofficial/minigotchi/issues/40)**

**This is a new feature to be added, this won't actually work with the v1.1.1(beta) release.**

- After everything starts up successfully, you can connect to the wifi network `minigotchi` using the password `dj1ch-minigotchi` with the webui being at http://192.168.4.1
- Happy hacking!

###
