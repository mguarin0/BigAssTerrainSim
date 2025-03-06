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

  std::vector<float> heightmap(width * height);

  if (band->RasterIO(GF_Read, 0, 0, width, height, heightmap.data(), width,
                     height, GDT_Float32, 0, 0) != CE_None) {
    std::cerr << "Error: Failed to read raster data" << std::endl;
    GDALClose(dataset);
    return false;
  }

  heightmap2D.assign(height, std::vector<float>(width));
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      heightmap2D[y][x] = heightmap[y * width + x];
    }
  }

  GDALClose(dataset);
  return true;
}

int RasterLoader::getWidth() const noexcept { return width; }
int RasterLoader::getHeight() const noexcept { return height; }
const std::vector<std::vector<float>>& RasterLoader::getHeightmap()
    const noexcept {
  return heightmap2D;
}
