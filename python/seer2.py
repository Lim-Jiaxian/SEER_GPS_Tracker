import csv
import numpy as np
import cartopy.crs as ccrs
#%matplotlib
import matplotlib.pyplot as plt
import cartopy.io.img_tiles as cimgt
from cartopy.mpl.ticker import LongitudeFormatter, LatitudeFormatter
import io,time
from urllib.request import urlopen, Request
from PIL import Image
plt.ion()
import pandas as pd
from sqlalchemy import create_engine
import sys  


# Replace the placeholder values with your actual database credentials
servername = 'localhost'
username = 'root'
userpwd = ''
dbname = 'geotrackdb'

connection_string = f'mysql+mysqlconnector://root:@localhost:3306/geotrackdb'
engine = create_engine(connection_string)
qry = 'SELECT * FROM records;'
df = pd.read_sql_query(qry,engine)

# You can use the DataFrame columns to access specific data
recordid = df['RecordID'].tolist()
secguard = df['SecGuardID'].tolist()
date = df['RecordTime'].tolist()
alts = df['Altitude'].tolist()
lats = df['Latitude'].tolist()
lons = df['Longitude'].tolist()
floors = df['Floor'].tolist()


def image_spoof(self, tile): 
    api_url = self._image_url(tile) # get the url of the street map API
    req = Request(api_url) # start request
    req.add_header('User-agent','Anaconda 3') # add user agent to request
    response = urlopen(req) #Open the URL and get the response
    im_data = io.BytesIO(response.read()) # Get image data 
    response.close() # Close the url
    img = Image.open(im_data) # open image using PIL
    img = img.convert(self.desired_tile_form)  # Convert the image format
    return img, self.tileextent(tile), 'lower' # Return the image, tile extent, and vertical alignment



cimgt.GoogleTiles.get_image = image_spoof # Spoof the web request for street map
osm_img = cimgt.GoogleTiles() # Download the spoofed street map using the GoogleTiles class
lat_zoom = 0.0005  # Decreased zoom for latitude
lon_zoom = 0.0025  # Decreased zoom for longitude

if lons and lats:
    extent = [np.min(lons) - lon_zoom, np.max(lons) + lon_zoom, np.min(lats) - lat_zoom, np.max(lats) + lat_zoom]

    fig = plt.figure(figsize=(14, 12), facecolor='#FCFCFC')
    ax1 = plt.axes(projection=osm_img.crs)
    ax1.set_title('GPS Tracker Map', fontsize=16)

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


for index in range(0, len(lons), 5):
        ax1.plot(lons[index], lats[index], markersize=10, marker='o', linestyle='',
                 color='Black', transform=ccrs.PlateCarree(), label='GPS Point')  # plot points

        annotation_text = f'Alt: {alts[index]:.2f} meters\nFloor: {floors[index]}'
        ax1.text(lons[index], lats[index], annotation_text,
                 fontsize=10, color='red', transform=ccrs.PlateCarree(), ha='right', va='bottom')

        plt.pause(0.01)
else:
    print("Error: lons or lats list is empty.")

plt.savefig('Images/plot.png')