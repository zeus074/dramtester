# dramtester V 4.2
Dram tester for 4116 and 4164/256 (now in beta testing for 4532)

Simple project taken from the base of @FozzTexx
with added support for 4116 memories and a status display.
This Tester supports 4164,41256,4116,*4532* and pin compatible Dram. 

![alt text](https://github.com/zeus074/dramtester/blob/main/IMG/IMG_6060.jpg)

**Components:**
C1,C4,C5,C6,C7: 100nF
C2: 10uF 25v
C3: 47uf 25v
D1: 1n4148
DC: MT3608 (DC-DC converter)
Disp: Display oled 128x32 i2c
H1,H2,H3: 3 pin Header-Male 2.54
R1-R13: 47Ohm 1/8w
R14,R16: 10kohm 1/4w
R15: 2.2kohm 1/4w
1x Arduino Nano
1x Zif 16 pin (or 16 pin socket)
1x 7760 (7660s)
1x tactile switch 6x6

**Directory**
Gerber : File for make the PCB
IMG : Tester's pictures
Firmware : Firmware for Arduino Nano (you can use avr commands like Arduino's IDE or an AVR programmer to write the HEX file)

**How it works**
First you need to flash the firmware on arduino of course!.
Before inserting the ram select the correct setting with the jumpers:
4116  4164-256 - (beta testing : 32k Low - 32k High)
...   ***
***   ***
***   ...

If the setting is on 4164-256 it is possible to select the type of memory (from 4164,41256,4532-L,4532-H) by briefly pressing the select / start key.
To start the test, keep the select / start key pressed for 1 sec.
memory 4116 has no selections.
After the test the result will be displayed, if there is an error the test will stop and the point of the error will be shown.

**How to burn the firmware**
You can use avrdude integrated on arduini via command line, or simply download a gui (if you use windows) like AVRDUDESS.
![alt text](https://github.com/zeus074/dramtester/blob/main/IMG/programming.jpg)
