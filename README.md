# Sunrise Siren 3000
### The most epic NeoPixel based alarm clock
<br>

## Introduction
The Sunrise Siren 3000 is the clock I have in front of my bed. Its job is to display time, temperature and humidity, but most importantly, wake me up whenever I have to wake up. I realize that you may not own a Sunrise Siren 3000 yourself, but I still like to make this project open-source :P  
This repository consists of two relevant parts of the clock:  
- the firmware of the clock, located in the `SunriseSirenFirmware` directory. It is flashed onto the ESP32 inside it by using `esptool` (version 11 and newer only) or the [Arduino IDE](https://www.arduino.cc/en/software);
- the control program, **Sunrise Siren Studio**, located in the `SunriseSirenStudio` directory. It is made with the [Gimp ToolKit](https://gtk.org/) (GTK) in C, making it only available to Linux (and WSL2) users. A `.deb` release is available for Debian/Ubuntu/Mint users. For other distributions, the program has to be compiled from source.

> [!CAUTION]
> Sunrise Siren 3000 passwords are NOT encrypted or hashed anywhere. **DO NOT USE A PASSWORD THAT YOU ALREADY USE SOMEWHERE ELSE!**

## Firmware setup
### Using esptool (version 11+ and USB cable only)
TODO

### Using the Arduino IDE (any version, USB cable or OTA)
1. Navigate to the `SunriseSirenFirmware` directory.
2. Launch the Arduino IDE and load the `SunriseSirenFirmware.ino` sketch.
> [!TIP]
> You can add the firmware sketch to your Arduino IDE sketchbook with a symlink:
> ```bash
> ln -s $(pwd) ~/Arduino
> ```
> Note that:
> - this command assumes you are in the `SunriseSirenFirmware` directory;
> - the location of your sketchbook may vary;
> - the sketch will open in read-only mode this way.
>
> Now, you can load the sketch easier.

3. Ensure that the ESP32 board is installed (if not, check [this tutorial](https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0/));
4. Ensure that the required libraries are installed:
    - **WiFiManager** *(by tzapu)*;
    - **FastLED** *(by Daniel Garcia)*;
    - **NTPClient** *(by Fabrice Weinberg)*;
    - **SHT2x** *(by Rob Tillaart)*.
5. Go to **Tools** > **Board** and ensure that it is set to "**ESP32 Dev Module**";
6. Click the Upload button.

### Congratulations, the Sunrise Siren 3000 firmware has been flashed successfully!
<br>

## Sunrise Siren Studio compilation
1. Ensure that the required libraries are installed.  
With `apt`, those can be installed by running:
    ```bash
    sudo apt install gcc make gzip libgtk-3-dev libcurl4-openssl-dev libjson-c-dev
    ```
    And with `dnf`:
    ```bash
    sudo dnf install gcc make gzip gtk3-devel libcurl-devel json-c-devel
    ```
2. Navigate to the `SunriseSirenStudio` directory.
3. Compile the program by running:
    ```bash
    make
    ```
> [!NOTE]
> If you want to test the program first (inside the repository, without installing it yet), it is necessary to refer to the compiled GSchema by using the `GSETTINGS_SCHEMA_DIR` environment variable when executing the binary:
> ```bash
> GSETTINGS_SCHEMA_DIR=$(pwd)/schema build/sunrise-siren-studio
> ```
> Note that:
> - This command assumes that you are in the `SunriseSirenStudio` directory.
> - The icon on the connection window may not be visible if the program (and so, its icon) is not installed yet.
4. To install the program on your computer, run:
    ```bash
    sudo make install
    ```
    Or, if you want to build your own `.deb` file and install it from there, run:
    ```bash
    make deb
    ```
5. To launch the program, simply search for "Sunrise Siren Studio" in the search bar of your computer!

### Congratulations, Sunrise Siren Studio is up and running now!
<br>

## Sunrise Siren Studio uninstallation
1. Simply run:
    ```bash
    sudo make uninstall
    ```
> [!WARNING]
> This also removes the saved credentials!
2. Optionally, you can remove all minified and compiled files from the repository:
    ```bash
    make clean
    ```
