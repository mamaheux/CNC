# control
This is a Qt interface to control the CNC.

## Dependencies
```bash
sudo apt install qt5-default libqt5charts5* libqt5serialport5* libqt5opengl5* mesa-common-dev libglu1-mesa-dev freeglut3-dev libgl-dev
```

## Linux Setup
Execute the following command `sudo visudo` and add the following line at the end.
```
%sudo ALL=(ALL) NOPASSWD: /sbin/poweroff, /sbin/reboot, /sbin/shutdown
```
