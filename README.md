# Katana FootController

## Work still in Progress - Code still messy 
This is an approach for an simple USB Controller for the Katana to replace and expand the GA FC.
It is possibel to config every switch to whatever function you like.

At the moment this is just for the MK2 (mkii), but its easy changeable to MK1 (mki).

## Status
### Working
- Change Channel
- Bank A/B
- Switch on/off Effect (also get Effect-Type/Color)
- Solo Boost
- Send/Return on/off

### Missing
- Switch Effect Type (red/green/yellow)
- Mute
- Tap
- Exp-Pedal enhancement

## Other Sources
### Code
To be very clear: I just adapted code from other people and just wrote some little code around it to fit my needs. Thanks a lot to the guys from VGuitar Forum and these github repos.

- https://www.vguitarforums.com/smf/index.php?topic=27185.0
- https://github.com/SteveObert/KatanaUSB_Midi_controller - Where I "stole" most of the code
- https://github.com/MrHaroldA/MS3 - This library is slightly modified and copied
- https://github.com/sixeight7/VController_v3 - Super advanced Midi Controller (also for the Katana) with huge amount of functionality.
- https://sourceforge.net/u/gumtownbassman/profile/ - Gumtown helped me a lot with the SYSEX Messages for the MK2
- https://github.com/snhirsch/katana-midi-bridge/ - This helped me a lot to understand whats going on with SYSEX

### Casing
- Font: https://www.1001freefonts.com/japanese-3017.font
- Box: https://www.festi.info/boxes.py/UnevenHeightBox?language=en 

## Material
- Arduino Mega (or clone)
- USB Host Shield / Breakout Board
- WS2812 / Neopixel in 12mm
- Switches (I am using these: https://www.thomann.de/de/tad_footswitch_momentary_spst.htm)
- Optional: https://www.thomann.de/de/neutrik_nausb_b.htm

## Pics
![Alt text](images/Image_top.jpg?raw=true "Top")
![Alt text](images/images_switches.jpg?raw=true "Switches")

