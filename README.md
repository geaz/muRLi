# muRLi
**muRLi** is a WiFi connected, programmable (mu)sic (R)eactive (Li)ght system.

## 3D Model
The model was designed in Fusion360. The STLs are included in the Thingiverse downloads and the Github repository. Furthermore there is an exported Fusion360 Archive File ready to download.

## muRLi

### BOM

- 3D Printed Parts (STL folder for models)
- Wemos D1 Mini
- 1x Pull-Down Resistor (I used a 1K resistor)
- Capacitor 680uF
- MAX4466
- [9x WS2812B LEDs (I used a strip and cut it)](https://www.amazon.de/dp/B01CDTED80)
- 1x 1x5 Dupont Male
- 4x M2x12mm screws
- [Stripboard](https://www.amazon.com/dp/B00C9NXP94)
- Wire
- Hotglue & superglue

### Firmware

Before building **muRLi** you should flash the firmware onto the Wemos D1 Mini. This way you are able to make sure that it is functional.

Open the project in [Visual Studio Code](https://code.visualstudio.com/). The project uses [PlatformIO](https://platformio.org/platformio-ide) to develop the firmware. Install the PlatformIO extension into Visual Studio Code, if not already done.

To flash the firmware onto the Wemos connect it to your PC and press the *Upload* button in the status bar.

![muRLi - Flash](https://raw.githubusercontent.com/geaz/muRLi/master/img/flash.png)

Aftererwards start the *Serial Monitor* and you should see some outputs like in the screenshot below. If you are able to see the output, the device is working and you are ready to build **muRLi**.

![muRLi - Serial](https://raw.githubusercontent.com/geaz/muRLi/master/images/serial.png)

### Build

Take the *Top* part and the *Frame* and glue them together with the superglue. I recommend to place something heavy on top to make sure that they bond well.

[PICTURE GLUED]

[PREPARE WIRES]

[SOLDER & MOUNT DISPLAY]

[SOLDER & MOUNT LED]

[SOLDER & MOUND MOD CONECTOR]

[SOLCER & MOUNT MIC]

[SOLDER VEROBOARD]

[SOLDER & MOUNT WEMOS]

[SET GAIN & FINISH MURLI]

## murLi MOD

### BOM (for one MOD)

- 3D Printed Parts (STL folder for models)
- 1x 24LC32A
- 1x 1x5 Dupont Female
- Enameled copper wire

### Build

### Create MODs

**murli** uses a modified version of [TinyScript](https://github.com/totalspectrum/tinyscript) to execute the MODs.
Every run the script will get some variables injected by **muRLi**.

- **v** *The calculated volume is a value between 0 and 100*
- **f** *The dominant frequency of the current loop*
- **lf** *The dominant frequency of the last loop*
- **minF** *The lowest frequency registered by muRLi - 130hz -> Lowest note for viola, mandola*
- **midF** *The middle frequency value - 1046hz -> Highest note reproducible by average female*
- **maxF** *The highest frequency registered by muRLi - 3140 -> Between highest note on a flute and on a 88-key piano* 

Scripts are able to return up to three different colors per run by calling the function *sCF*.
The methods has the following signature:

```sCF(int frame, int hexColor)```

A script which would never change the color would look like:

```sCF(1, 0xFFFFFF)```

For more advanced examples look into the *mods* folder.

### Write MODs

In the *mods* folder is a python script to write the mods onto the cartridges. Please make sure, that Python3 is installed.
Connect **muRLi** to your machine and execute the script like this:

```
python writeMod.py COM4 simple.murli
```

The script will tell you, if something went wrong. **muRLi** will show a *MOD saved*, if everything went well.
If you run the script without any parameters, it will list all available COM ports.

## muRLi Nodes

**Nodes** are devices which are able to connect to a *websocket* provided by **muRLi** or a closer available other **node** via WiFi. Because every **node** is also an Access Point and a Websocket Server it is possible to span a pretty big mesh of **muRLi Nodes**.

This repository provides a *naive* example for such a node in form of a battery powered device. But it is possible to create other devices like for example wall mounted LED devices which will connect to **muRLi** to create some nice color effects. Or desk lamp like devices. Just play with your imagination and come up with some cool devices. All you have to do is to take a *Wemos D1 Mini* connect some LEDs and flash the **muRLi** node firmware on it.

### BOM (For one small battery powered muRLi node)

- 3D Printed Parts (STL folder for models)
- Wemos D1 Mini
- TP4056 charging board (make sure you buy one rated for 1A and with integrated load protection)

### Firmware

### Build

### OTA Update