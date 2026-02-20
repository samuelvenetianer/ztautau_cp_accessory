echo 'Setting up ROOT and Python'

## Installation on lxplus (to be adapted for other computer systems)
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
lsetup "root 6.28.10-x86_64-el9-gcc13-opt"
export PYTHONPATH=$PWD:${PYTHONPATH}
