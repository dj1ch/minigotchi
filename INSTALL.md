# Install guide
Here you can find out how to install minigotchi on the esp8266, or the raspberry pi pico. The esp8266 microcontroller has more support than the pico at the moment. They can be both built using arduino, ~~but you can build manually with the pico, assuming that you have all the libraries from the arduino esp8266 repo. Again, building with Arduino IDE is an option for the raspberry pi pico.~~ The support for this has been dropped, as this is not maintained nor tested by me.
#### Building using Arduino IDE/Web editor
I believe that you can also use the raspberry pi pico and run arduino .ino files. It would be much easier to do this then to build it yourself manually.
- Download the latest release [here](https://github.com/Pwnagotchi-Unofficial/minigotchi/releases).
- Unzip the file
- Navigate to the `minigotchi.ino` file in the unzipped folder, and open it up
- At the line that says:

```cpp
raw.init("bssid of ap you will listen on", channel number); // set the settings here, ("BSSID", channel)
```

Replace the `"bssid of ap you will listen on"` with your actual BSSID(in the quotations), and the `channel` with the channel you prefer(not in quotations). Note that the pwnagotchi will constantly switch channels so you should go check the channels in the pwnagotchi's web UI.
- There should also be a line that says:

```cpp
deauthAttack.addToWhitelist("SSID"); // add your ssid(s) here
deauthAttack.addToWhitelist("ANOTHER_SSID");
```
Replace the `SSID` and `ANOTHER_SSID` with the ssid's you want whitelisted. You can add more in this manner by copying and pasting this into the setup function
```cpp
deauthAttack.addToWhitelist("YOUR_SSID_HERE");
```
- Save and exit the file.
- Right click on the folder(should be called minigotchi) then archive it, making it back into a zip file. If you need to rename it, rename it
- Go to the [arduino web editor](https://create.arduino.cc/editor), then sign in/create an account
- Import the zip file by clicking the button that looks like an upload button, then selecting the minigotchi zip file that you edited **Note: you may also use the Arduino IDE, as the website seems to have compile issues**
- Select the board as `ESP8266 WEMOS(LOLIN) D1 mini Lite`, and select the port it is plugged into(if you haven't already, plug in the board)
- Click on the upload button(arrow pointing to the left).
- You can click on the monitor button on the sidebar to see the serial monitor. Make sure the baud rate is `115200`.
- **Compile errors happen with the Web editor, so you can just use the Arduino IDE app. Make sure you install all the correct libraries(ArduinoJson), and the correct [boards](https://github.com/esp8266/Arduino). See this issue I made [here](https://github.com/Pwnagotchi-Unofficial/minigotchi/issues/40)**
- After everything starts up successfully, you can connect to the wifi network `minigotchi` using the password `dj1ch-minigotchi` with the webui being at http://192.168.4.1
- Happy hacking!
####
