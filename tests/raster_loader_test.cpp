#include "../include/raster_loader.h"

#include <gtest/gtest.h>

// Test Fixture for RasterLoader
class RasterLoaderTest : public ::testing::Test {
 protected:
  RasterLoader raster;
};

// Test loading a valid GeoTIFF file
TEST_F(RasterLoaderTest, LoadValidHeightmap) {
  std::string testFile =
      "/usr/src/data/"
      "GMTED_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_"
      "15arc_15arc_15arc_15arc_15arc_15arc_15arc_30arc_30arc_30arc_30arc_30arc_"
      "30arc_30arc_n47RrIKWcISms4JK0BEy/"
      "usgs_gmted2010_7pnt5arcsec_breakline_emphasis_n47RrIKWcISms4JK0BEy.tiff";

  ASSERT_TRUE(raster.loadHeightmap(testFile))
      << "Failed to load valid heightmap";

  EXPECT_GT(raster.getWidth(), 0);
  EXPECT_GT(raster.getHeight(), 0);

  const auto& heightmap = raster.getHeightmapHost();
  EXPECT_EQ(heightmap.size(), raster.getWidth() * raster.getHeight());
}

// Test loading a non-existent file
TEST_F(RasterLoaderTest, LoadInvalidFile) {
  std::string invalidFile = "non_existent.tif";
  EXPECT_FALSE(raster.loadHeightmap(invalidFile))
      << "RasterLoader should fail for non-existent files";
}

// Test checking that heightmap data is correctly stored
TEST_F(RasterLoaderTest, HeightmapDataCheck) {
  std::string testFile =
      "/usr/src/data/"
      "GMTED_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_"
      "7pnt5arc_15arc_15arc_15arc_15arc_15arc_15arc_15arc_30arc_30arc_"
      "30arc_30arc_30arc_30arc_30arc_n47RrIKWcISms4JK0BEy/"
      "usgs_gmted2010_7pnt5arcsec_breakline_emphasis_n47RrIKWcISms4JK0BEy."
      "tiff";  // Replace with actual test file path

  ASSERT_TRUE(raster.loadHeightmap(testFile)) << "Failed to load heightmap";

  const auto& heightmap = raster.getHeightmapHost();
  int width = raster.getWidth();
  int height = raster.getHeight();

  EXPECT_GT(heightmap[0], 0.0f) << "First pixel should have a value";
  EXPECT_GT(heightmap[(height - 1) * width + (width - 1)], 0.0f)
      << "Bottom-right pixel should have a value";
}

// Test copying heightmap to GPU
TEST_F(RasterLoaderTest, CopyToGPU) {
  std::string testFile =
      "/usr/src/data/"
      "GMTED_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_7pnt5arc_"
      "15arc_15arc_15arc_15arc_15arc_15arc_15arc_30arc_30arc_30arc_30arc_30arc_"
      "30arc_30arc_n47RrIKWcISms4JK0BEy/"
      "usgs_gmted2010_7pnt5arcsec_breakline_emphasis_n47RrIKWcISms4JK0BEy.tiff";

  ASSERT_TRUE(raster.loadHeightmap(testFile)) << "Failed to load heightmap";

  // Copy to GPU
  raster.copyToGPU();

  // Check if GPU vector has the same size
  const thrust::device_vector<float>& heightmapDevice =
      raster.getHeightmapDevice();
  EXPECT_EQ(heightmapDevice.size(), raster.getWidth() * raster.getHeight())
      << "GPU heightmap size should match the CPU heightmap size";
}
