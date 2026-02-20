"""
This script defines plotting functions.
These functions are called in run_plotting.py.

Note: this script relies on plotly and the user may need 
to run the following lines in terminal before use:

    pip install plotly
    pip install --upgrade kaleido
    plotly_get_chrome

"""
 
#import required packages
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
import sys
import csv
import math
import plotly.express as px

def two_D_plot (reco_file, truth_file, csv_path, fig_filename, fig_path):

  """
  A function for plotting a 2D colormap of two variables.
  The x-axis and y-axis are two different variables.
  The color represents the number of entries

  Parameters:
    reco_file (str): name of csv file with data for reco psi
    truth_file (str): name of csv file with data for truth psi
    csv_path (str): folder with csv files
    fig_filename (str): filename for output plot
    fig_path (str): folder path for output plot

  Returns:
    No explicit return.
    Will write and save figure.
  """

  # construct paths  
  psi_reco_file = csv_path + reco_file
  psi_truth_file = csv_path + truth_file
  fig_file = fig_path + fig_filename
  
  print("Reco data: \n", psi_reco_file)
  print("Truth data: \n", psi_truth_file)
  print("Plot will be saved here: \n", fig_file)

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

