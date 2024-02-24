# Install guide

Here you can find out how to install minigotchi on the ESP8266. The ESP8266 microcontroller has more support than the Pico at the moment, and I don't plan on supporting the Pico anymore.

### Building using Arduino IDE

- Download the latest release [here](https://github.com/Pwnagotchi-Unofficial/minigotchi/releases).

- Unzip the file

- Navigate to the `minigotchi.ino` file in the unzipped folder, and open it up with the text editor of your choice. 

- At the line that says:

```cpp
raw.init("fo:od:ba:be:fo:od", 1); // set the settings here, ("BSSID", channel)
```

Replace the `"fo:od:ba:be:fo:od"` with your actual BSSID(in the quotations), and the `1` with the channel you prefer(not in quotations). Note that the WiFi network you're listening on should be on a specific WiFi channel anyway, each AP is on a specific one. 

- There should also be a line that says:

```cpp
deauth.add("fo:od:ba:be:fo:od"); // add your ssid(s) here
deauth.add("fo:od:ba:be:fo:od");
```

Replace the `fo:od:ba:be:fo:od` with the ssid's you want whitelisted. You can add more in this manner by copying and pasting this into the setup function

```cpp
deauth.add("YOUR_SSID_HERE_OR_BSSID_IDC_REALLY");
```

- Save and exit the file.

- Download the [Arduino IDE](https://https://www.arduino.cc/en/software), preferably the latest version.

- In the IDE, go to `File` > `Preferences`, then in the `Additional board manager URLS`, paste the link `https://arduino.esp8266.com/stable/package_esp8266com_index.json` to add the ESP8266 board(s) to the board manager

- Click `Ok` and plug the board into your computer. It should blink, make sure it is receiving the correct amount of voltage, too much will burn it(I learned that the hard way)

- Open up the minigotchi folder through the IDE by pushing `Ctrl O`, or by going to `File` > `Open`, then selecting the folder the .ino is in. If other tabs don't show up, along with `minigotchi.ino`, make sure to copy and paste the files into the same directory/folder as that lone `.ino` file. It won't be able to compile if that happens. 

- Install the following dependancies with the library manager: `ArduinoJson`, etc with all with their dependancies(Please install all of them for them to work correctly).

- Select the board as `ESP8266 WEMOS(LOLIN) D1 mini Lite`, and select the port it is plugged into(if you haven't already, plug in the board)
- Click on the upload button(arrow pointing to the left). 

**OR**

- Go to `Sketch` > `Export Compiled Binary` to generate a `.bin` file to flash to the board, which also works. This can also be done with `Alt Ctrl S`.

**Note: if you get any errors, let me know ASAP with a github issue, sending me a discord message, or perhaps even messaging me through my portfolio site.**

- You can click on the *Serial Monitor* button on the top bar to see the serial monitor output. Make sure the baud rate is `115200`. You can also use https://serial.huhn.me/ as a serial monitor, I also recommend using this! 

- Happy hacking!

###
