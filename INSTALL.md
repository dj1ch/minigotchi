#### Install guide(I just use this to test if it runs, but you can try as well)
From the look of it, you might need to install the micropython firmware and put the files through the IDE(In this case Thonny is preferred).
1. Copy the files, uf2 files first, then the python files, although i just copy and paste them in the editor and save them to the raspberry pi pico as main.py
2. Unplug then replug
3. Connect to the pico via a shell, and the code should run, but the lua stuff is a work in progress
####
#### Note
The pico itself doesn't really have a hard disk. The "disk" that gets mounted on your computer is just a dummy. Whatever you put on there will be copied to the actual memory/ram.
