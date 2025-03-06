#include "../include/raster_loader.h"

#include <iostream>

RasterLoader::RasterLoader() { GDALAllRegister(); }
RasterLoader::~RasterLoader() {}

bool RasterLoader::loadHeightmap(const std::string& filepath) {
  GDALDataset* dataset =
      static_cast<GDALDataset*>(GDALOpen(filepath.c_str(), GA_ReadOnly));
  if (!dataset) {
    std::cerr << "Error: Failed to open file: " << filepath << std::endl;
    return false;
  }

  GDALRasterBand* band = dataset->GetRasterBand(1);
  if (!band) {
    std::cerr << "Error: Failed to get raster band" << std::endl;
    GDALClose(dataset);
    return false;
  }

  width = band->GetXSize();
  height = band->GetYSize();
  std::cout << "Loaded TIFF Dimensions: " << width << " x " << height
            << std::endl;

  heightmapHost.resize(width * height);

  if (band->RasterIO(GF_Read, 0, 0, width, height, heightmapHost.data(), width,
                     height, GDT_Float32, 0, 0) != CE_None) {
    std::cerr << "Error: Failed to read raster data" << std::endl;
    GDALClose(dataset);
    return false;
  }

  GDALClose(dataset);
  return true;
}

void RasterLoader::copyToGPU() {
  heightmapDevice = heightmapHost;  // Copy host vector to device vector
}

int RasterLoader::getWidth() const noexcept { return width; }
int RasterLoader::getHeight() const noexcept { return height; }
const std::vector<float>& RasterLoader::getHeightmapHost() const noexcept {
  return heightmapHost;
}
const thrust::device_vector<float>& RasterLoader::getHeightmapDevice()
    const noexcept {
  return heightmapDevice;
}
