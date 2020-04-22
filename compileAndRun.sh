PTFE_NEVENTS=1000
PTFE_RANDOMSEED=111
PTFE_VISUAL=1

source /Users/sdporzio/Software/geant4.10.06.p01/install/bin/geant4.sh
cd build
cmake ../
make -j 4 && ./main ${PTFE_NEVENTS} ${PTFE_RANDOMSEED} ${PTFE_VISUAL}