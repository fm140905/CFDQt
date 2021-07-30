# Simple GUI application for demonstrating real-time radiation flux calculation using CFD
The program allows the user to move the detector using sliders and a gamma spectrum is calculated using CFD and rendered on the screen in real-time.

## Prerequisites
- Qt 5.13 or higher

## Build
  ```bash
  mkdir -p build
  cd build
  qmake -config release ../gui.pro
  make
  ```
## Run
```bash
    cd ..
    build/cfdqt/CFDQt
```
## Demo
<img src="Demo/zoom_0.gif" width="800" height="400" />
