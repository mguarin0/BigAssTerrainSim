#pragma once

#include <thrust/device_vector.h>
#include <thrust/for_each.h>
#include <thrust/tuple.h>

#include <cmath>

class AgentSimulator {
 public:
  AgentSimulator(int num_agents, int width, int height);

  void initializeAgents();
  void advanceSimulation();

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
