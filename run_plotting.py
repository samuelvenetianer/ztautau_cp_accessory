"""
This script creates plots using csv files as input. 
Accessory functions live in plotting_func.py.
"""

from plotting_func import two_D_plot

# Define paths for input csvs and output figures
csv_folder_path = "/afs/cern.ch/user/s/svenetia/ztautau_cp_accessory/out-files/csv/"
fig_folder_path = "/afs/cern.ch/user/s/svenetia/ztautau_cp_accessory/plots/"

# Generate plots
two_D_plot("psiRecoPol.csv", "psiTruthPol.csv", csv_folder_path, "2D_Pol.png", fig_folder_path)
# two_D_plot("psiRecoUnPol.csv", "psiTruthUnPol.csv", csv_folder_path, "2D_UnPol.png", fig_folder_path)