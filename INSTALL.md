#### Install guide
As of right now, nothing. If you want to test the firmware and your own code, you can copy the uf2 files, the boot.py, and your own code.lua to the pico, and mess with the firmware. We might need to use circuitpython firmware, but I'm trying to implement it.
####
#### What might make this a bit of a challenge
The pico itself doesn't really have a hard disk. The "disk" that gets mounted on your computer is just a dummy. Whatever you put on there will be copied to the actual memory/ram. Keep in mind the memory will only run/save uf2 files, though that I cannot exactly confirm. Let me know if you find anything. 
