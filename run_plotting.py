from plotting import two_D_plot

csv_folder_path = "/afs/cern.ch/user/s/svenetia/ztautau_cp_accessory/"
fig_folder_path = "/afs/cern.ch/user/s/svenetia/ztautau_cp_accessory/"

# run plotting.py

# two_D_plot(psi_reco_filename, psi_truth_filename, csv_folder_path, fig_name (include .png), fig_folder_path)
two_D_plot("psiRecoPol.csv", "psiTruthPol.csv", csv_folder_path, "2D_Pol.png", fig_folder_path)
two_D_plot("psiRecoUnPol.csv", "psiTruthUnPol.csv", csv_folder_path, "2D_UnPol.png", fig_folder_path)

# # Having trouble editing binsize - seems to not adjust based on nbinsx/nbinsy