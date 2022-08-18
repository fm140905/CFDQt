#/bin/bash

echo "Run neutron CFD with 1E6 NPS..."
./neutronSim
echo "CFD done."
tallyFile="output_neutron/tally.txt"
if [ -f $tallyFile ]; then
    echo "The CFD result should be similar to the MCNP one."
    # cd output_neutron
    # python compare.py
    # cd ..
    ./compare neutron
    echo "Test done."
else
    echo "Test failed."
fi