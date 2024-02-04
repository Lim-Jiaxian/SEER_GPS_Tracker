import csv
import numpy as np
import cartopy.crs as ccrs
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import cartopy.io.img_tiles as cimgt
from cartopy.mpl.ticker import LongitudeFormatter, LatitudeFormatter
import io
from urllib.request import urlopen, Request
from PIL import Image
plt.ion()
import pandas as pd
from sqlalchemy import create_engine
import sys
import os

# Replace the placeholder values with your actual database credentials
servername = 'localhost'
username = 'root'
userpwd = ''
dbname = 'geotrackdb'

connection_string = f'mysql+mysqlconnector://root:@localhost:3306/geotrackdb'
engine = create_engine(connection_string)
qry = 'SELECT * FROM records;'
df = pd.read_sql_query(qry, engine)

# You can use the DataFrame columns to access specific data
recordid = df['RecordID'].tolist()
secguards = df['SecGuardID'].unique()  # Get unique SecGuardIDs
date = df['RecordTime'].tolist()
alts = df['Altitude'].tolist()
lats = df['Latitude'].tolist()
lons = df['Longitude'].tolist()
floors = df['Floor'].tolist()

def image_spoof(self, tile): 
    api_url = self._image_url(tile)  # get the url of the street map API
    req = Request(api_url)  # start request
    req.add_header('User-agent', 'Anaconda 3')  # add user agent to request
    response = urlopen(req)  # Open the URL and get the response
    im_data = io.BytesIO(response.read())  # Get image data
    response.close()  # Close the url
    img = Image.open(im_data)  # open image using PIL
    img = img.convert(self.desired_tile_form)  # Convert the image format
    return img, self.tileextent(tile), 'lower'  # Return the image, tile extent, and vertical alignment

cimgt.GoogleTiles.get_image = image_spoof  # Spoof the web request for street map
osm_img = cimgt.GoogleTiles()  # Download the spoofed street map using the GoogleTiles class
lat_zoom = 0.0005  # Decreased zoom for latitude
lon_zoom = 0.0025  # Decreased zoom for longitude

# Create a directory for individual plots
individual_plots_directory = '../seer/Images/Individual_Plots/'
# individual_plots_directory = 'Images/Individual_Plots/'
os.makedirs(individual_plots_directory, exist_ok=True)

for secguard in secguards:
    secguard_df = df[df['SecGuardID'] == secguard]

    if lons and lats:
        extent = [
            np.min(secguard_df['Longitude']) - lon_zoom,
            np.max(secguard_df['Longitude']) + lon_zoom,
            np.min(secguard_df['Latitude']) - lat_zoom,
            np.max(secguard_df['Latitude']) + lat_zoom
        ]

        fig, ax1 = plt.subplots(figsize=(14, 12), facecolor='#FCFCFC', subplot_kw={'projection': osm_img.crs})
        ax1.set_title(f'GPS Tracker Map - SecGuardID {secguard}', fontsize=16)

        # Set the extents of the map
        ax1.set_extent(extent)

        # Set longitude and latitude indicators
        ax1.set_xticks(np.linspace(extent[0], extent[1], 7), crs=ccrs.PlateCarree())
        ax1.set_yticks(np.linspace(extent[2], extent[3], 7)[1:], crs=ccrs.PlateCarree())

        # Format the longitude and latitude tick labels
        lon_format = LongitudeFormatter(number_format='0.1f', degree_symbol='', dateline_direction_label=True)
        lat_format = LatitudeFormatter(number_format='0.1f', degree_symbol='')

        # Set the major formatters for longitude and latitude
        ax1.xaxis.set_major_formatter(lon_format)
        ax1.yaxis.set_major_formatter(lat_format)

        # Set tick label font sizes
        ax1.xaxis.set_tick_params(labelsize=14)
        ax1.yaxis.set_tick_params(labelsize=14)

        # Empirical calculation of scale based on zoom
        scale = np.ceil(-np.sqrt(2) * np.log(np.divide((extent[1] - extent[0]) / 2.0, 350.0)))

        # Ensure scale does not exceed 19
        scale = (scale < 20) and scale or 19

        # Add the spoofed OSM image to the map with the specified zoom level
        ax1.add_image(osm_img, int(scale + 1))

 
        floor_colormap = plt.cm.get_cmap('viridis', len(secguard_df['Floor'].unique()))

        for index in range(0, len(secguard_df['Longitude']), 5):
            floor_color = floor_colormap(secguard_df['Floor'].iloc[index] / len(secguard_df['Floor'].unique()))
            ax1.plot(secguard_df['Longitude'].iloc[index], secguard_df['Latitude'].iloc[index], markersize=10,
                    marker='o', linestyle='', color=floor_color, transform=ccrs.PlateCarree(), label='GPS Point')

            plt.pause(0.1)  # Adjust the pause duration

        # Create a legend
        handles = []

        for floor in secguard_df['Floor'].unique():
            floor_color = floor_colormap(floor / len(secguard_df['Floor'].unique()))
            handles.append(plt.Line2D([0], [0], marker='o', color=floor_color, label=f'Floor {floor}', markersize=10, markerfacecolor=floor_color))

        ax1.legend(handles=handles, loc='upper left', bbox_to_anchor=(1, 1), title='Legend', fontsize=10)

        # Save individual plots
        plt.savefig(f'{individual_plots_directory}/plot_SecGuardID_{secguard}.png')
        plt.show()


    else:
        print("Error: lons or lats list is empty.")


