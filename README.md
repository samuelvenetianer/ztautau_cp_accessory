# ztautau_cp_accessory
This script takes an ntuple created in xTauFramework (https://gitlab.cern.ch/mcochran/xTauFramework/-/tree/add-ztt-hadhad-channel?ref_type=heads) and calculates truth and reco psi for the hadhad channel. It also contains plotting functionality.

Before running anything, setup ROOT and python with:
    `source setup.sh`

`hadhad_psi.cc`:
This script reads an ntuple and outputs root files for truth and reco psi as well as a csv file that can be read into run_plotting.py

In main block, select a sample by uncommenting the polarized or unpolarized block. Make sure input and output file paths are correct.

Before running, don't forget to compile with:
    `make`
And run in terminal as:
    `./hadhad_psi`

`run_plotting.py.`
This script calls functions defined in `plotting_func.py`, inputs csv output from `hadhad_psi.cc`, and outputs a plot in .png format. To generate plots, update the path to reco/truth csvs and output figure in `run_plotting.py` and change output file name in the line that actually executes the function. More specific changes to the plot (i.e. axes titles) can be made in `plotting_func.py`.

To run in terminal:
    `python run_plotting.py`
