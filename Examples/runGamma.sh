#/bin/bash

echo "Run gamma CFD with 1E7 NPS..."
./gammaSim
echo "CFD done."
tallyFile="output_gamma/tally.txt"
if [ -f $tallyFile ]; then
    cd output_gamma
    echo "The CFD result should be similar to the MCNP one."
    python compare.py
    cd ..
    echo "Test done."
else
    echo "Test failed."
fi