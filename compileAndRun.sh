PTFE_NEVENTS=40000
PTFE_RANDOMSEED=111
PTFE_VISUAL=0

export PTFE_ANAROOT="anaexample.root"
export PTFE_SURFTYPE="2"


if [ "$GAL" == "/Users/sdporzio/Work/MOUNT/GALILEO" ]
then
  source /Users/sdporzio/Software/geant4.10.06.p01/install/bin/geant4.sh
else
  source /global/project/projectdirs/lz/users/sdporzio/setup.sh
  source /cvmfs/lz.opensciencegrid.org/external/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/bin/geant4.sh
fi
cd build
cmake ../
make -j 4 && ./main ${PTFE_NEVENTS} ${PTFE_RANDOMSEED} ${PTFE_VISUAL}