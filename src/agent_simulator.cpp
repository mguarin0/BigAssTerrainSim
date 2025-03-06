#include "../include/agent_simulator.h"

#include <thrust/execution_policy.h>
#include <thrust/random.h>

struct RandomPositionInit {
  int width, height;
  unsigned int seed;

  __host__ __device__ void operator()(thrust::tuple<float&, float&> t) {
    thrust::default_random_engine rng(seed);
    thrust::uniform_real_distribution<float> distX(0, width - 1);
    thrust::uniform_real_distribution<float> distY(0, height - 1);

    thrust::get<0>(t) = distX(rng);
    thrust::get<1>(t) = distY(rng);
  }
};

AgentSimulator::AgentSimulator(int num_agents, int width, int height)
    : num_agents(num_agents),
      width(width),
      height(height),
      d_x(num_agents),
      d_y(num_agents),
      d_heading(num_agents),
      d_speed(num_agents) {}

void AgentSimulator::initializeAgents() {
  thrust::for_each(
      thrust::make_zip_iterator(thrust::make_tuple(d_x.begin(), d_y.begin())),
      thrust::make_zip_iterator(thrust::make_tuple(d_x.end(), d_y.end())),
      RandomPositionInit{width, height,
                         static_cast<unsigned int>(time(nullptr))});
}

struct MoveAgents {
  int width, height;

  __host__ __device__ void operator()(
      thrust::tuple<float&, float&, float, float> t) {
    float& x = thrust::get<0>(t);
    float& y = thrust::get<1>(t);
    float heading = thrust::get<2>(t);
    float speed = thrust::get<3>(t);

    x += cos(heading) * speed;
    y += sin(heading) * speed;

    // Keep within bounds
    x = fmaxf(0, fminf(x, width - 1));
    y = fmaxf(0, fminf(y, height - 1));
  }
};

void AgentSimulator::advanceSimulation() {
  thrust::for_each(
      thrust::make_zip_iterator(thrust::make_tuple(
          d_x.begin(), d_y.begin(), d_heading.begin(), d_speed.begin())),
      thrust::make_zip_iterator(thrust::make_tuple(
          d_x.end(), d_y.end(), d_heading.end(), d_speed.end())),
      MoveAgents{width, height});
}
