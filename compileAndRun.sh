<<<<<<< HEAD
PTFE_NEVENTS=10
=======
PTFE_NEVENTS=500000
>>>>>>> 78e61f059ea90b0aa2e3d3d3da150934c3a70ae2
PTFE_RANDOMSEED=111
PTFE_VISUAL=1

export PTFE_ANAROOT="sim.root"
<<<<<<< HEAD
export PTFE_SURFTYPE="3"
export PTFE_DEPTH="10" #nm
=======
export PTFE_SURFTYPE="1"
>>>>>>> 78e61f059ea90b0aa2e3d3d3da150934c3a70ae2


if [ "$GAL" == "/Users/sdporzio/Work/MOUNT/GALILEO" ]
then
  source /Users/sdporzio/Software/geant4.10.06.p01/install/bin/geant4.sh
else
  source /cvmfs/lz.opensciencegrid.org/external/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/bin/geant4.sh
fi
cd build
cmake ../
make -j 4 && ./main ${PTFE_NEVENTS} ${PTFE_RANDOMSEED} ${PTFE_VISUAL}
