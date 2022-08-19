@echo off

IF "%~1" == "gamma" (
  echo "Run gamma CFD with 1E7 NPS..."
  .\Release\gammaSim.exe
  echo "CFD done."
  SET tallyFile=output_gamma\tally.txt
  IF EXIST  "%tallyFile%" (
    echo "The CFD result should be similar to the MCNP one."
    .\Release\compare.exe gamma
    echo "Test done."
  ) ELSE (
    echo "Test failed."
  )
) ELSE (
  IF "%~1" == "neutron" (
    echo "Run neutron CFD with 1E6 NPS..."
    .\Release\neutronSim.exe
    echo "CFD done."
    SET tallyFile=output_neutron\tally.txt
    IF EXIST "%tallyFile%" (
      echo "The CFD result should be similar to the MCNP one."
      .\Release\compare.exe neutron
      echo "Test done."
    ) ELSE (
      echo "Test failed."
    )
  ) ELSE (
    echo "Invalid input."
  )
)