#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/random.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include "../include/agent_simulator.h"
#include "../include/raster_loader.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <path_to_tiff>" << std::endl;
    return EXIT_FAILURE;
  }

  RasterLoader raster;
  if (!raster.loadHeightmap(argv[1])) {
    return EXIT_FAILURE;
  }

  raster.copyToGPU();  // Move heightmap to GPU
  std::cout << "Heightmap copied to GPU." << std::endl;

  int width = raster.getWidth();
  int height = raster.getHeight();
  const std::vector<float>& heightmap =
      raster.getHeightmapHost();  // CPU copy of the heightmap

  // Initialize agents
  int num_agents = 100;
  AgentSimulator sim(num_agents, width, height);
  sim.initializeAgents();  // Randomize positions
  std::cout << "Agents initialized with random positions." << std::endl;

  // Host vectors for tracking positions
  std::vector<float> x_host(num_agents);
  std::vector<float> y_host(num_agents);

  // Device vectors for speed and heading updates
  thrust::device_vector<float> delta_speed(num_agents);
  thrust::device_vector<float> delta_heading(num_agents);

  // Random number generator
  thrust::default_random_engine rng(time(nullptr));

  for (int i = 0; i < 100; ++i) {
    // Define distributions for movement updates
    thrust::uniform_real_distribution<float> speed_dist(
        -0.1f, 0.1f);  // Speed change per step
    thrust::uniform_real_distribution<float> heading_dist(
        -0.05f, 0.05f);  // Heading change per step

    // Generate random speed and heading changes
    thrust::host_vector<float> h_delta_speed(num_agents);
    thrust::host_vector<float> h_delta_heading(num_agents);

    for (int j = 0; j < num_agents; ++j) {
      h_delta_speed[j] = speed_dist(rng);
      h_delta_heading[j] = heading_dist(rng);
    }

    // Copy to device
    delta_speed = h_delta_speed;
    delta_heading = h_delta_heading;

    // Advance simulation
    sim.advanceSimulation(delta_speed, delta_heading);

    // Copy updated positions back to host
    thrust::copy(sim.getX().begin(), sim.getX().end(), x_host.begin());
    thrust::copy(sim.getY().begin(), sim.getY().end(), y_host.begin());

    for (size_t j = 0; j < x_host.size(); ++j) {
      int agent_x = static_cast<int>(x_host[j]);
      int agent_y = static_cast<int>(y_host[j]);

      // Ensure agent stays within the terrain bounds
      agent_x = std::max(0, std::min(agent_x, width - 1));
      agent_y = std::max(0, std::min(agent_y, height - 1));

      // Retrieve elevation from the 1D heightmap (row-major order)
      float elevation = heightmap[agent_y * width + agent_x];

      std::cout << "Step " << i + 1 << ": (" << "Agent " << j << ": ("
                << x_host[j] << ", " << y_host[j]
                << ") Elevation: " << elevation << "\n";
    }
  }

  return EXIT_SUCCESS;
}
