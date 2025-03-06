#include <cstdlib>
#include <iostream>

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

  // Initialize agents
  int num_agents = 100;
  AgentSimulator sim(num_agents, raster.getWidth(), raster.getHeight());
  sim.initializeAgents();
  std::cout << "Agents initialized." << std::endl;

  // Choose an agent to track (e.g., agent index 0)
  int agentIndex = 0;
  std::vector<float> x_host(sim.getX().size());
  std::vector<float> y_host(sim.getY().size());

  for (int i = 0; i < 10; ++i) {
    sim.advanceSimulation();

    // Copy all agent positions from GPU to CPU
    thrust::copy(sim.getX().begin(), sim.getX().end(), x_host.begin());
    thrust::copy(sim.getY().begin(), sim.getY().end(), y_host.begin());

    std::cout << "Step " << i + 1 << " Positions:\n";
    for (size_t j = 0; j < x_host.size(); ++j) {
      std::cout << "Agent " << j << ": (" << x_host[j] << ", " << y_host[j]
                << ")\n";
    }
  }

  return EXIT_SUCCESS;
}
