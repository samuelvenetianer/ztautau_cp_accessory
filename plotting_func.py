#import required packages
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
import sys
import csv
import math
import plotly.express as px

# before starting, may need to run in terminal:
    # pip install plotly
    # pip install --upgrade kaleido
    # plotly_get_chrome

def two_D_plot (reco_file, truth_file, csv_path, fig_filename, fig_path):

  # construct paths  
  psi_reco_file = csv_path + reco_file
  psi_truth_file = csv_path + truth_file
  fig_file = fig_path + fig_filename
  
  print("Pulling reco results here: ", psi_reco_file)
  print("Pulling truth results here: ", psi_truth_file)
  print("Plot will be saved here: ", fig_file)

  # open .csvs as dataFrames and combine
  df_reco = pd.read_csv(psi_reco_file, delimiter=",", names = ["psi_reco"])
  df_truth = pd.read_csv(psi_truth_file, delimiter=",", names = ["psi_truth"])
  psi_reco = df_reco["psi_reco"]
  psi_truth = df_truth["psi_truth"]
  psi_df = pd.DataFrame(data=[psi_reco, psi_truth]).T

  # plotting colormap where color denotes number of entries

  print("Plotting...")
  fig2 = px.density_heatmap(psi_df, x="psi_reco", 
                            y="psi_truth", 
                            nbinsx=28, 
                            nbinsy=28, 
                            #title = fig_filename
                            )
  fig2.update_layout(
    xaxis_title="Psi Reco",
    yaxis_title="Psi Truth",
    coloraxis_colorbar_title="Entries",
    font=dict(
        size=18,
        )
  )
  fig2.write_image(fig_file)
  print("Plot saved! All done.")

