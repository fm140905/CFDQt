#/bin/bash

echo "Run neutron CFD with 1E6 NPS..."
./neutronSim
echo "CFD done."
tallyFile="output_neutron/tally.txt"
if [ -f $tallyFile ]; then
    cd output_neutron
    echo "The CFD result should be similar to the MCNP one."
    python compare.py
    cd ..
    echo "Test done."
else
    echo "Test failed."
fi