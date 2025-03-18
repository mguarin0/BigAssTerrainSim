#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/random.h>

#include <cstdlib>
#include <fstream>
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

  raster.copyToGPU();
  std::cout << "Heightmap copied to GPU." << std::endl;

  int width = raster.getWidth();
  int height = raster.getHeight();
  const std::vector<float>& heightmap = raster.getHeightmapHost();

  int num_agents = 100;
  AgentSimulator sim(num_agents, width, height);
  sim.initializeAgents();
  std::cout << "Agents initialized with random positions." << std::endl;

  std::vector<float> x_host(num_agents);
  std::vector<float> y_host(num_agents);

  thrust::device_vector<float> delta_speed(num_agents);
  thrust::device_vector<float> delta_heading(num_agents);

  thrust::default_random_engine rng(time(nullptr));

  std::ofstream outfile("agent_positions.csv");
  outfile << "step,agent_id,x,y,elevation\n";  // CSV Header

  for (int i = 0; i < 100; ++i) {  // Increase steps for a longer animation
    thrust::uniform_real_distribution<float> speed_dist(-0.1f, 0.1f);
    thrust::uniform_real_distribution<float> heading_dist(-0.05f, 0.05f);

    thrust::host_vector<float> h_delta_speed(num_agents);
    thrust::host_vector<float> h_delta_heading(num_agents);

    for (int j = 0; j < num_agents; ++j) {
      h_delta_speed[j] = speed_dist(rng);
      h_delta_heading[j] = heading_dist(rng);
    }

    delta_speed = h_delta_speed;
    delta_heading = h_delta_heading;

    sim.advanceSimulation(delta_speed, delta_heading);

    thrust::copy(sim.getX().begin(), sim.getX().end(), x_host.begin());
    thrust::copy(sim.getY().begin(), sim.getY().end(), y_host.begin());

    for (size_t j = 0; j < x_host.size(); ++j) {
      int agent_x =
          std::max(0, std::min(static_cast<int>(x_host[j]), width - 1));
      int agent_y =
          std::max(0, std::min(static_cast<int>(y_host[j]), height - 1));

      float elevation = heightmap[agent_y * width + agent_x];
      outfile << i << "," << j << "," << x_host[j] << "," << y_host[j] << ","
              << elevation << "\n";
    }
  }

  outfile.close();
  std::cout << "Agent positions saved to agent_positions.csv\n";

  return EXIT_SUCCESS;
}
