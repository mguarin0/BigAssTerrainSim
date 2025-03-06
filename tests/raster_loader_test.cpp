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

  const auto& heightmap = raster.getHeightmap();
  EXPECT_EQ(heightmap.size(), raster.getHeight());
  EXPECT_EQ(heightmap[0].size(), raster.getWidth());
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

  const auto& heightmap = raster.getHeightmap();
  EXPECT_GT(heightmap[0][0], 0.0f);  // Ensure first pixel has a value
}
