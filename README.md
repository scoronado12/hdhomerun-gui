# hdhomerun-gui


![Main View](.github/screenshot.png)

Third Party HD HomeRun Client made using qt. It is able to pull down Channel data via HTTP GET requests that are publically avaliable

## Why?

There is no native client on Linux, leaving Desktop users to install Kodi or any other PVR client to simply watch TV.

# Dependencies

- QT 5
- mpv Media Player
- glibc
- curl
- libhdhomerun

## License

GNU GPLv3

# Building

Download the sources.

    git clone https://github.com/scoronado12/hdhomerun-gui

Change into the working directory

    cd ./hdhomerun-gui
    
Run `qmake` to generate a makefile

    qmake


Lastly, run `make`

    make
