# Demonstration of CFD in a Simple Geometry

## Prerequisites
- Install qt5 development packages
```bash
sudo apt-get install qt5-default
```
- python for plotting

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
