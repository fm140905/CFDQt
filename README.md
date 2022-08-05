# Simple GUI application for demonstrating real-time radiation flux calculation using CFD
User can move the detector using sliders. A gamma spectrum at current detector position is calculated using CFD and rendered on the screen in real-time.

## Prerequisites
- Qt 5.13 or higher
- OpenGL 3.0 or higher

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
<img src="Demo/demo.gif" width="800" height="400" />
