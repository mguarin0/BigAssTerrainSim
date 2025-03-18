import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import imageio
import rasterio
import os

# Load terrain heightmap
terrain = rasterio.open(
    "/usr/src/data/GMTED_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_15arc_15arc_15arc_15arc_15arc_15arc_15arc_30arc_30arc_30arc_30arc_30arc_30arc_30arc_n47RrIKWcISms4JK0BEy/usgs_gmted2010_7pnt5arcsec_breakline_emphasis_n47RrIKWcISms4JK0BEy.tiff"
).read(1)

# Load agent positions
df = pd.read_csv("/usr/src/agent_positions.csv")

# Get simulation metadata
num_steps = df["step"].max() + 1
terrain_height, terrain_width = terrain.shape

# Create output directory for frames
frame_dir = "/usr/src/frames"
os.makedirs(frame_dir, exist_ok=True)
frame_paths = []

# Set up figure with correct aspect ratio
fig, ax = plt.subplots(figsize=(10, 10 * (terrain_height / terrain_width)), dpi=100)
ax.set_axis_off()  # Hide axes for clean image output

# Generate frames manually
for step in range(num_steps):
    ax.clear()
    ax.imshow(terrain, cmap="gray", origin="lower", extent=[0, terrain_width, 0, terrain_height])
    ax.set_axis_off()

    # Plot agents for the current step
    step_data = df[df["step"] == step]
    ax.scatter(step_data["x"], step_data["y"], c="red", edgecolor="black", s=5)

    # Save the frame
    frame_path = f"{frame_dir}/frame_{step:03d}.png"
    frame_paths.append(frame_path)
    plt.savefig(frame_path, dpi=100, bbox_inches='tight', pad_inches=0)

# Create GIF
gif_path = "agent_simulation.gif"
imageio.mimsave(gif_path, [imageio.imread(f) for f in frame_paths], fps=10)

print(f"Animation saved as {gif_path}")
