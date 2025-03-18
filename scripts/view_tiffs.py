import rasterio
import numpy as np
import matplotlib.pyplot as plt

filename = "/usr/src/data/GMTED_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_15arc_15arc_15arc_15arc_15arc_15arc_15arc_30arc_30arc_30arc_30arc_30arc_30arc_30arc_n47RrIKWcISms4JK0BEy/usgs_gmted2010_7pnt5arcsec_breakline_emphasis_n47RrIKWcISms4JK0BEy.tiff"  # "/usr/src/data/2024-10-01-00_00_2024-10-01-23_59_Sentinel-2_Quarterly_Mosaics_True_Color_Cloudless.tiff"
# Load GeoTIFF
with rasterio.open(filename) as dataset:
    heightmap = dataset.read(1)  # Read the first band (elevation)

# Print basic info
print(
    f"Shape: {heightmap.shape}, Min Elevation: {np.min(heightmap)}, Max Elevation: {np.max(heightmap)}"
)

# Plot the heightmap
plt.imshow(heightmap, cmap="terrain")
plt.colorbar(label="Elevation (m)")
plt.savefig("ok.png")


from PIL import Image

# Normalize to 0-255 range
heightmap_norm = (
    255 * (heightmap - np.min(heightmap)) / (np.max(heightmap) - np.min(heightmap))
).astype(np.uint8)

# Save as PNG
Image.fromarray(heightmap_norm).save("heightmap.png")


with rasterio.open(filename) as dataset:
    print(dataset.profile)  # Metadata (e.g., CRS, dtype, transform)
    print(dataset.bounds)  # Geographic bounding box
    print(dataset.crs)  # Coordinate Reference System
