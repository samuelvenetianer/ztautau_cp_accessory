#import required packages
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
import sys
import csv
import math

# define psi files
psi_reco_file = "/afs/cern.ch/user/s/svenetia/ztautau_cp_accessory/psi_reco.csv"
psi_truth_file = "/afs/cern.ch/user/s/svenetia/ztautau_cp_accessory/psi_truth.csv"
fig_path = "/afs/cern.ch/user/s/svenetia/ztautau_cp_accessory/2D.png"

# open txt
df_reco = pd.read_csv(psi_reco_file, delimiter=",", names = ["psi_reco"])
df_truth = pd.read_csv(psi_truth_file, delimiter=",", names = ["psi_truth"])

psi_reco = df_reco["psi_reco"]
psi_truth = df_truth["psi_truth"]

# print(psi_reco)
# print(psi_truth)
# print(len(psi_reco))
# print(len(psi_truth))

# plotting (simple scatter)
fig=plt.figure()
plt.scatter(psi_reco, psi_truth)
plt.xlabel("Psi Reco")
plt.ylabel("Psi Truth")
plt.savefig(fig_path, bbox_inches="tight")
plt.close()

# plotting (color map where entries is color)

