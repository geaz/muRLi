[![CC licensed](https://img.shields.io/badge/license-Creative%20Commons%20BY--NC--SA-orange)](https://raw.githubusercontent.com/geaz/muRLi/master/LICENSE.md)  
[![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/A0A01MQZP)

# muRLi
**muRLi** is a WiFi connected and programmable (mu)sic (R)eactive (Li)ght system. It supports custom *MOD* cartridges which contain executable scripts. These scripts are able to influence the light patterns produced by **muRLi**.

Furthermore **muRLi** creates a naive mesh including a *websocket*. **muRLiN**s (muRLi nodes) are able to connect to this mesh and will create an additional access point and websocket for other nodes to connect to. This way nodes not in range of **muRLi** are also able to join the mesh as long as at least one **muRLiN** is in range. The nodes are able to receive *commands* created by **muRLi**. The *commands* contain color information to which the **muRLiN**s will react.

*Everything combined, it is possible to span a mesh of WiFi connected devices which are synchronously reacting to music.*

![muRLi](https://raw.githubusercontent.com/geaz/muRLi/master/img/cover.jpg)

# Table of Contents
1. [3D Model](#3d-model)
2. [muRLi](#murli)  
    a) [BOM](#bom)  
    b) [Firmware](#firmware)  
    c) [Build](#build)
3. [MODs](#mods)  
    a) [BOM (for one MOD)](#BOM-for-one-MOD)  
    b) [Build MOD](#mod-build)  
    c) [Create MODs](#create-mods)  
    d) [Write MODs](#write-mods)
4. [muRLiN](#murlin)  
    a) [BOM (for one small battery powered muRLiN)](#node-bom)  
    b) [Firmware](#node-firmware)  
    c) [Build](#node-build)  
    d) [OTA Updates](#node-ota)

## 3D Model
The models were designed in Fusion 360. The exported STLs are included in the Github repository (*stl* folder). Furthermore there are Fusion 360 archive files ready to download (*cad* folder).

## muRLi

### BOM

- 3D Printed Parts
- 1x Wemos D1 Mini
- 1x 1K Resistor
- 1x Capacitor 680uF
- 1x MAX4466
- 1x 1x5 DuPont Male Pin Header
- [9x WS2812B LEDs (I used a strip and cut it)](https://www.amazon.de/dp/B01CDTED80)
- 4x M2x12mm screws
- 4x M3x4mm screws
- [Stripboard](https://www.amazon.com/dp/B00C9NXP94)
- Wire
- Hotglue & Superglue

### Firmware

Before building **muRLi** you should flash the firmware onto the Wemos D1 Mini. This way you are able to make sure that it is functional. Open the project in [Visual Studio Code](https://code.visualstudio.com/). The project uses [PlatformIO](https://platformio.org/platformio-ide) to develop the firmware. Install the PlatformIO extension into Visual Studio Code, if not already done. 

To flash the firmware onto the Wemos connect it to your PC, make sure that the **-DBUILD_MURLI** build flag is active in the *platformio.ini* file and press the *Upload* button in the status bar.

![muRLi - Flash](https://raw.githubusercontent.com/geaz/muRLi/master/img/flash.png)

Afterwards start the *serial monitor* and you should see some outputs like in the screenshot below. If you are able to see the output, the device is working and you are ready to build **muRLi**.

![muRLi - Serial](https://raw.githubusercontent.com/geaz/muRLi/master/img/serial.png)

### Build

First of all, make yourself comfortable with the following schematics. It shows you all parts of **muRLi** which are necessary to connect to each other. We will use a piece of stripboard for each wire which is not directly connected to the Wemos D1 Mini. Mostly to provide power to each part.

![muRLi - Schematics](https://raw.githubusercontent.com/geaz/muRLi/master/img/muRLI-circuit.png)

Now that you made yourself comfortable with the wiring, take the *Top* part and the *Frame* and glue them together with the superglue. I recommend to place something heavy on top to make sure that they bond well.

![muRLi - Frame Glued](https://raw.githubusercontent.com/geaz/muRLi/master/img/frame-glued.jpg)

Solder wires to your display and mount it to the *Top* part.

![muRLi - Display](https://raw.githubusercontent.com/geaz/muRLi/master/img/display.jpg)

To diffuse the LEDs a bit, the *middle* part has a small slot for a paper insert. Take a bit of paper and cut it into a narrow stripe and insert it into the slot.

![muRLi - LED Paper](https://raw.githubusercontent.com/geaz/muRLi/master/img/led-paper.jpg)

![muRLi - LED Inserted](https://raw.githubusercontent.com/geaz/muRLi/master/img/led-paper-insert.jpg)

Solder three wires to your nine LED stripe and insert it into the *middle* part, too - **pay attention to the data line direction**. I bent the cables away from the back of the shell to avoid any possible interference between the microphone cables and the LEDs.

![muRLi - LED Inserted](https://raw.githubusercontent.com/geaz/muRLi/master/img/led-insert.jpg)

Solder three wires to your microphone and attach it with hotglue to the *Bottom* part of the shell.

![muRLi - Mic](https://raw.githubusercontent.com/geaz/muRLi/master/img/mic.jpg)

At this point I advise you to build your first [MOD](#mods). A *MOD* will help to align the MOD connector correctly in the *bottom* shell.   

**Playing jingle ...**

Now that your fist **MOD** is done, take the stripboard and create one 4x7 and one 2x5 sized piece. Picture below as a reference - pay attention to the copper line directions.

![muRLi - Stripboard](https://raw.githubusercontent.com/geaz/muRLi/master/img/stripboard.jpg)

Solder the DuPont male pin header to one row of the 2x5 stripboard piece and five wires to the second row. The wires should be directed to the opposite side of the pin header and should sit flush. Otherwise the connector will not sit correctly in the cartridge slot. 

Now insert the *MOD* connector into the slot of the *Bottom* part of the shell and try to insert your *MOD*. If they fit, hotglue the connector to the shell. This way the connector and the *MOD*s will align perfectly.

![muRLi - MOD Connector](https://raw.githubusercontent.com/geaz/muRLi/master/img/mod-connector.jpg)

Take the 4x7 stripboard you prepared before and solder all necessary components and wires as shown in the image below. The ground wires are only indicated (read the red comment) to keep the image tidy. Also solder every other wire like indicated in the above schematics.

![muRLi - Veroboard](https://raw.githubusercontent.com/geaz/muRLi/master/img/murRLi-Veroboard.png)

Mount the Wemos D1 Mini to the *Bottom* part of the shell.

![muRLi - Mounted](https://raw.githubusercontent.com/geaz/muRLi/master/img/mounted.jpg)

Power the Wemos and insert a *MOD*. You should see the *decibel* in the top right of the display. Set the gain of you microphone with a small screwdriver. It should float around -25dB in a silent room. If done, place the stripboard into the shell, place one of the *divider* parts on top of the *bottom* part, then place the *middle* part on top of it. Followed by an additional *divider* and the *top* part.
Insert the *bolts* into the four holes and use the M2x12mm screws to close the shell.

## MODs

### BOM (for one MOD)

- 3D Printed Parts
- 1x 24LC32A
- 1x 1x5 DuPont Female Connector
- Superglue
- Wire

### <a name="mod-build"></a> Build

Building a MOD cartridge is quite easy. Just take a bit of wire, the *23LC32A* and the DuPont connector and solder it as shown in the schematics.

![muRLi MOD - Schematics](https://raw.githubusercontent.com/geaz/muRLi/master/img/muRLI-circuit-mod.png)

Here is a picture of my first one. Not pretty, but it works.

![muRLi MOD](https://raw.githubusercontent.com/geaz/muRLi/master/img/MOD-Chip.jpg)

Take a bit of superglue and attach the DuPont connector to the cartridge shell. Make sure that the *ground pin* is oriented to the side of the shell **without the gap** and that it sits in the middle of the gap! Otherwise you will have a hard time to insert it into the MOD slot.

![muRLi MOD](https://raw.githubusercontent.com/geaz/muRLi/master/img/MOD-Cart.jpg)

Now just glue the two shell halves together and you are done.

### Create MODs

**murli** uses a modified version of [TinyScript](https://github.com/totalspectrum/tinyscript) to execute the MODs.
Every run the script will get some variables injected by **muRLi**.

- **v** *The calculated volume is a value between 0 and 100*
- **f** *The dominant frequency of the current loop*
- **lf** *The dominant frequency of the last loop*
- **minF** *The lowest frequency registered by muRLi - 130hz -> Lowest note for viola, mandola*
- **midF** *The middle frequency value - 1046hz -> Highest note reproducible by average female*
- **maxF** *The highest frequency registered by muRLi - 3140 -> Between highest note on a flute and on a 88-key piano* 

The volume and frequency are zero, if **muRLi** recognized silence for at least five seconds. This way a MOD
could calculate some ambient lights for the silence phase.

Scripts are able to return up to two different colors per run by calling the *sCF* method.
It has the following signature:

```sCF(int colorFrame, int hexColor)```

A script which would never change the color would look like:

```sCF(0, 0xFFFFFF)```

For more advanced examples look into the *mods* folder.

### Write MODs

In the *mods* folder is a python script to write the mods onto the cartridges. Please make sure, that Python3 is installed.
Connect **muRLi** to your machine, insert a cartridge and execute the script like this:

```
python writeMod.py COM4 simple.murli
```

The script will tell you, if something went wrong. **muRLi** will show a *MOD saved*, if everything went well.
If you run the script without any parameters, it will list all available COM ports.

## muRLiN

**muRLiN**s are devices which are able to connect to the *websocket* provided by **muRLi** or a closer available other **muRLiN** via WiFi. Because every **muRLiN** is also an access point and a websocket server it is possible to span a pretty big mesh of **muRLiN**s.

This repository provides a *naive* example for a **muRLiN** in form of a battery powered device. But it is possible to create other devices like for example wall mounted LED devices which will connect to **muRLi** to create some nice color effects. Or desk lamp like devices. Just play with your imagination and come up with some cool devices. All you have to do is to take a *Wemos D1 Mini* connect some LEDs and flash the **muRLiN** firmware on it.

### <a name="node-bom"></a> BOM (For one small battery powered muRLiN)

- 3D Printed Parts
- Wemos D1 Mini
- TP4056 charging board (make sure you buy one with integrated load protection)

### <a name="node-firmware"></a> Firmware

### <a name="node-build"></a> Build

### <a name="node-ota"></a> OTA Update