from sentinelhub import SHConfig, SentinelHubRequest, DataCollection, MimeType, bbox_to_dimensions, BBox
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

# Set up Sentinel Hub API credentials
config = SHConfig()
config.sh_client_id = "your_client_id"
config.sh_client_secret = "your_client_secret"

# Define area of interest (longitude, latitude, EPSG:4326)
bbox = BBox(bbox=[-122.5, 37.0, -122.0, 37.5], crs="EPSG:4326")
resolution = 10  # 10m per pixel
size = bbox_to_dimensions(bbox, resolution=resolution)

# Create request for Sentinel-2 data
request = SentinelHubRequest(
    evalscript="""
        // Sentinel Hub Evalscript for RGB
        function setup() {
            return {
                input: ["B4", "B3", "B2"],
                output: { bands: 3 }
            };
        }
        function evaluatePixel(sample) {
            return [sample.B4, sample.B3, sample.B2];
        }
    """,
    input_data=[
        {
            "dataCollection": DataCollection.SENTINEL2_L2A,
            "timeRange": ("2024-02-01", "2024-02-28"),
            "responses": [{"identifier": "default", "format": MimeType.TIFF}]
        }
    ],
    bbox=bbox,
    size=size,
    config=config
)

# Get image data
image = request.get_data()[0]

# Convert to grayscale if needed
gray_image = np.mean(image, axis=-1)

# Save image as GeoTIFF
img = Image.fromarray(gray_image.astype(np.uint8))
img.save("sentinel_10m.tif")

# Show the image
plt.imshow(image)
plt.show()
