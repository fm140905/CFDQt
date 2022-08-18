#/bin/bash

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