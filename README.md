#  SIMH sound project

This is a fork of the [SIMH computer simulator](https://github.com/open-simh/simh), trying to add real or realistic sounds from disk drives and other computer devices to the simulation using the SDL2 sdl_mixer extension.

There are two reasons I want sound for SIMH (the sound of the running computer and its devices: fans, disks, floppys, tape drive, ... - as true as possible):
- an objective reason:
Imagine you're sitting in front of a computer with some software running which doesn't show any sign of progress.
In real life you would at least hear the disk being accessed (if its not a modern SSD), if its a SIMH simulated computer you would hear NOTHING. I want audible feedback from the computer I'm working on (at least if its a classic one)!
- a sentimental and/or nostalgic reason:
I want to "feel" the computer with as many senses as possible in order to have the most realistic/authentic experience possible.
Works starts with the DEC RL02 removable disk pack drive (because I've got two of them).

I would very much appreciate if owners of other classic computer devices would contribute sound samples of their devices, preferably, but not exclusively DEC components.

Following a most-wanted list:
- RK07
- any classic DEC 9-track tape drive
- RM03
- RP04
- RP05
- RM80
- RP06
- RM05
- RP07

## The MSCP disk drive problem
You may have noticed that there are no MSCP disk drives (RAxx, RDxx) on my wish list.

It's not that I don't like these drives, but the problem is that the MSCP disk simulation in SIMH is based on the MSCP protocol, not the drive itself. MSCP only deals with block transfers to and from disks and there is no information about what the disk needs to do to make these transfers possible, i.e. no information about seeking (moving the disk heads between tracks), which is what causes disk sounds during access.

Therefore, there is currently no way to generate realistic disk access sounds for MSCP disks (and this will probably never change), only artificial sounds that mimic the real sounds when reading or writing.
