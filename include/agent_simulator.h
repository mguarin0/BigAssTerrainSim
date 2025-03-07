#pragma once

#include <thrust/device_vector.h>

class AgentSimulator {
 public:
  AgentSimulator(int num_agents, int width, int height);

  void initializeAgents();  // Randomize initial positions
  void advanceSimulation(const thrust::device_vector<float>& delta_speed,
                         const thrust::device_vector<float>& delta_heading);

  const thrust::device_vector<float>& getX() const { return d_x; }
  const thrust::device_vector<float>& getY() const { return d_y; }

 private:
  int num_agents;
  int width, height;

  thrust::device_vector<float> d_x;
  thrust::device_vector<float> d_y;
  thrust::device_vector<float> d_heading;
  thrust::device_vector<float> d_speed;
};
