<!--
 * @Description: 
 * @Author: Ming Fang
 * @Date: 1969-12-31 18:00:00
 * @LastEditors: Ming Fang
 * @LastEditTime: 2022-08-18 18:10:26
-->
# Demonstration of CFD in a Simple Geometry

## Prerequisites
- C++17. Make sure your C++ compiler supports C++17.
- CMake 3.14 or higher. If CMake is not installed, install it following the instructions [here](https://cmake.org/install/).

- Gnuplot 5.2.6+. 
  - Ubuntu + gcc:
    ```bash
    sudo apt install gnuplot
    ```
  - MacOS + Clang:
    ```bash
    sudo brew install gnuplot
    ```
  - Windows + MSVC. Download Gnuplot from [www.gnuplot.info](http://www.gnuplot.info) and install it. If you're using the Gnuplot installer, make sure you mark the option "Add application directory to your PATH environment variable"

## Build
  ```bash
  mkdir -p build
  cd build
  cmake ..
  cmake --build .
  ```
## Run Examples
```bash
cd ../Examples
# run the gamma simulation, ~ 10 s
./runGamma.sh
# run the neutron simulation, ~ 10 s
./runNeutron.sh 
```
