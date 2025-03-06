#pragma once

#include <gdal.h>
#include <gdal_priv.h>
#define THRUST_DEVICE_SYSTEM THRUST_DEVICE_SYSTEM_CPP
#include <thrust/device_vector.h>

#include <string>
#include <vector>

class RasterLoader {
 public:
  RasterLoader();
  ~RasterLoader();

  bool loadHeightmap(const std::string& filepath);

  int getWidth() const noexcept;
  int getHeight() const noexcept;
  const std::vector<float>& getHeightmapHost() const noexcept;
  const thrust::device_vector<float>& getHeightmapDevice() const noexcept;

  void copyToGPU();

 private:
  int width{0};
  int height{0};

  std::vector<float> heightmapHost;              // 1D host storage
  thrust::device_vector<float> heightmapDevice;  // GPU storage
};
