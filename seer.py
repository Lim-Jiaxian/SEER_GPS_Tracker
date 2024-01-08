#Import necessary libs
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

################################
#  parsing the GPS coordinates #
################################


arduino_data = []
with open('python/CP_GPS.csv','r') as dat_file:
    reader = csv.reader(dat_file)
    # Iterate over each row in the CSV file and append it to the arduino_data list
    for row in reader:
        arduino_data.append(row)
        
header = arduino_data[0]  # header text

# Initialize empty lists to store date, time, latitude, and longitude data
date, time_vec, lats, lons = [], [], [], []

for row in arduino_data[1:]:
    date.append(row[0])
    time_vec.append(row[1])
    lats.append(float(row[2]))  
    lons.append(float(row[3]))  



#######################################
#      Formatting the Cartopy plot    #
#######################################


cimgt.GoogleTiles.get_image = image_spoof # Spoof the web request for street map
osm_img = cimgt.GoogleTiles() # Download the spoofed street map using the GoogleTiles class

fig = plt.figure(figsize=(14,12),facecolor='#FCFCFC')# Open a matplotlib figure for the GPS tracker map
ax1 = plt.axes(projection=osm_img.crs) # Set up the map projection using the coordinate reference system (CRS) of the street map
ax1.set_title('GPS Tracker Map',fontsize=16)
lat_zoom = 0.0005  # Decreased zoom for latitude
lon_zoom = 0.0025  # Decreased zoom for longitude
# Define the map view bounds based on GPS data
extent = [np.min(lons) - lon_zoom, np.max(lons) + lon_zoom, np.min(lats) - lat_zoom, np.max(lats) + lat_zoom]

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



#######################################
#         Plot the GPS points         #
#######################################

    
for index in range(0,len(lons),5):
    ax1.plot(lons[index],lats[index], markersize=10,marker='o',linestyle='',
             color='Black',transform=ccrs.PlateCarree(),label='GPS Point') # plot points
    transform = ccrs.PlateCarree()._as_mpl_transform(ax1) # set transform for annotations
    plt.pause(1) # pause between point plots
    plt.savefig('Images/map.png', format='png', dpi=300) # save the figure to a file
   
