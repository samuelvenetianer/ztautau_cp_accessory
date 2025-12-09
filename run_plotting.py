from plotting import two_D_plot

# set file names
psi_reco_filename = "psi_reco.csv"
psi_truth_filename = "psi_truth.csv"
fig_name = "test" #Polarized or Unpolarized

csv_folder_path = "/afs/cern.ch/user/s/svenetia/ztautau_cp_accessory/"
fig_folder_path = "/afs/cern.ch/user/s/svenetia/ztautau_cp_accessory/"

# run plotting.py

two_D_plot(psi_reco_filename, psi_truth_filename, csv_folder_path, fig_name, fig_folder_path)

# # Having trouble editing binsize - seems to not adjust based on nbinsx/nbinsy