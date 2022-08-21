# Demonstration of CFD in a Simple Geometry

## Prerequisites
- C++17. Make sure your C++ compiler supports C++17.
- CMake 3.14 or higher. If CMake is not installed, install it following the instructions [here](https://cmake.org/install/).

- Gnuplot 5.2.6+. 
  - Ubuntu + gcc:
    ```bash
    sudo apt install gnuplot
    ```
  - Windows + MSVC:
    Download Gnuplot from [www.gnuplot.info](http://www.gnuplot.info) and install it. Make sure you mark the option "Add application directory to your PATH environment variable"


## Build
  ```bash
  mkdir build
  cd build
  cmake ..
  cmake --build . --config Release
  ```
## Run Examples
- Ubuntu + gcc:
  ```bash
  cd ../Examples
  # run the gamma simulation
  ./run.sh gamma
  # run the neutron simulation
  ./run.sh neutron 
  ```
- Windows + MSVC:
  ```cmd
  cd ../Examples
  run.bat gamma
  run.bat neutron 
  ```
