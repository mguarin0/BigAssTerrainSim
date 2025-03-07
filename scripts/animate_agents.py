import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import imageio
from matplotlib.animation import FuncAnimation

import rasterio
import numpy as np

data = rasterio.open("/usr/src/data/GMTED_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_15arc_15arc_15arc_15arc_15arc_15arc_15arc_30arc_30arc_30arc_30arc_30arc_30arc_30arc_n47RrIKWcISms4JK0BEy/usgs_gmted2010_7pnt5arcsec_breakline_emphasis_n47RrIKWcISms4JK0BEy.tiff").read(1)
np.savetxt("terrain_heightmap.csv", data, delimiter=",")

# Load terrain heightmap (replace with actual data)
terrain = np.loadtxt("terrain_heightmap.csv", delimiter=",")  # Assuming terrain is saved as CSV

# Load agent positions
df = pd.read_csv("../agent_positions.csv")

# Get simulation metadata
num_steps = df["step"].max() + 1
num_agents = df["agent_id"].max() + 1
terrain_width, terrain_height = terrain.shape

# Initialize figure
fig, ax = plt.subplots(figsize=(8, 6))
ax.set_xlim(0, terrain_width)
ax.set_ylim(0, terrain_height)

# Show terrain as grayscale
ax.imshow(terrain, cmap="gray", origin="lower")

# Initialize agent scatter plot
agent_scatter = ax.scatter([], [], c="red", edgecolor="black", s=5)

def update(frame):
    """Update function for each frame in animation"""
    step_data = df[df["step"] == frame]
    agent_scatter.set_offsets(np.c_[step_data["x"], step_data["y"]])
    return agent_scatter,

# Create animation
anim = FuncAnimation(fig, update, frames=num_steps, interval=100, blit=True)

# Save animation as GIF
anim.save("agent_simulation.gif", writer="pillow", fps=10)

print("Animation saved as agent_simulation.gif")
