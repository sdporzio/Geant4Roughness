if [ "$GAL" == "/Users/sdporzio/Work/MOUNT/GALILEO" ]
then
  source /Users/sdporzio/Software/geant4.10.06.p01/install/bin/geant4.sh
fi
cd build
cmake ../
make -j 4
