#include <cstdlib>
#include <iostream>

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

  const auto& heightmap2D = raster.getHeightmap();

  std::cout << "Heightmap Data (First 3 Rows):" << std::endl;
  for (size_t y = 0; y < std::min<size_t>(3, heightmap2D.size()); ++y) {
    for (size_t x = 0; x < std::min<size_t>(5, heightmap2D[y].size()); ++x) {
      std::cout << heightmap2D[y][x] << " ";
    }
    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}
