#include "../include/agent_simulator.h"

#include <thrust/execution_policy.h>
#include <thrust/for_each.h>
#include <thrust/random.h>
#include <thrust/transform.h>

#include <cmath>

// Functor to initialize agents with random positions
struct RandomPositionInit {
  int width, height;

  __host__ __device__ void operator()(thrust::tuple<float&, float&> t) {
    // Generate a unique seed per thread
    unsigned int seed =
        (unsigned int)(reinterpret_cast<uintptr_t>(&t) ^ clock());
    thrust::default_random_engine rng(seed);
    thrust::uniform_real_distribution<float> distX(0, width - 1);
    thrust::uniform_real_distribution<float> distY(0, height - 1);

    thrust::get<0>(t) = distX(rng);
    thrust::get<1>(t) = distY(rng);
  }
};

// Functor to apply external updates to speed and heading
struct ApplyUpdates {
  __host__ __device__ void operator()(thrust::tuple<float&, float> t) {
    thrust::get<0>(t) += thrust::get<1>(t);  // Apply delta to speed or heading
  }
};

// Functor to move agents based on speed and heading
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

    // Keep agents within bounds
    x = fmaxf(0, fminf(x, width - 1));
    y = fmaxf(0, fminf(y, height - 1));
  }
};

AgentSimulator::AgentSimulator(int num_agents, int width, int height)
    : num_agents(num_agents),
      width(width),
      height(height),
      d_x(num_agents),
      d_y(num_agents),
      d_heading(num_agents, 0.0f),  // Default heading
      d_speed(num_agents, 1.0f) {   // Default speed
}

void AgentSimulator::initializeAgents() {
  thrust::for_each(
      thrust::make_zip_iterator(thrust::make_tuple(d_x.begin(), d_y.begin())),
      thrust::make_zip_iterator(thrust::make_tuple(d_x.end(), d_y.end())),
      RandomPositionInit{width, height});
}

void AgentSimulator::advanceSimulation(
    const thrust::device_vector<float>& delta_speed,
    const thrust::device_vector<float>& delta_heading) {
  // Apply external updates to speed and heading
  thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(
                       d_speed.begin(), delta_speed.begin())),
                   thrust::make_zip_iterator(
                       thrust::make_tuple(d_speed.end(), delta_speed.end())),
                   ApplyUpdates());

  thrust::for_each(thrust::make_zip_iterator(thrust::make_tuple(
                       d_heading.begin(), delta_heading.begin())),
                   thrust::make_zip_iterator(thrust::make_tuple(
                       d_heading.end(), delta_heading.end())),
                   ApplyUpdates());

  // Move agents based on updated speed and heading
  thrust::for_each(
      thrust::make_zip_iterator(thrust::make_tuple(
          d_x.begin(), d_y.begin(), d_heading.begin(), d_speed.begin())),
      thrust::make_zip_iterator(thrust::make_tuple(
          d_x.end(), d_y.end(), d_heading.end(), d_speed.end())),
      MoveAgents{width, height});
}
