#pragma once

#include <gdal.h>
#include <gdal_priv.h>

#include <string>
#include <vector>

class RasterLoader {
 public:
  RasterLoader();
  ~RasterLoader();

  bool loadHeightmap(const std::string& filepath);

  int getWidth() const noexcept;
  int getHeight() const noexcept;
  const std::vector<std::vector<float>>& getHeightmap() const noexcept;

 private:
  int width{0};
  int height{0};
  std::vector<std::vector<float>> heightmap2D;
};
