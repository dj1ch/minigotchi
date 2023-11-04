# Second install guide
### Manual building
This is how you can manually build the minigotchi files for the pico. In order to put them on, make sure to put the raspberry pi pico into BOOTSEL mode(hold down the BOOTSEL button and plug it into your computer).

- Clone this repository using git
`git clone https://github.com/Pwnagotchi-Unofficial/minigotchi.git`

- Go to the appropriate directory
`cd minigotchi/pico/manual`

- Make a build directory, then go into it
`mkdir build && cd build`

- Build the .uf2 files
`cmake .. && make`

- From here the files should generate. This guide assumes you are on a linux machine. If you want to cover a windows guide, open up a PR 
