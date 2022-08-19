#/bin/bash

if [ "$1" == "gamma" ]; then
    echo "Run gamma CFD with 1E7 NPS..."
    ./gammaSim
    echo "CFD done."
    tallyFile="output_gamma/tally.txt"
    if [ -f $tallyFile ]; then
        echo "The CFD result should be similar to the MCNP one."
        # cd output_gamma
        # python compare.py
        # cd ..
        ./compare gamma
        echo "Test done."
    else
        echo "Test failed."
    fi
elif [ "$1" == "neutron" ]; then
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
else
    echo "Invlid input."
fi
