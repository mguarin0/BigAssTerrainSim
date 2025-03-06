# BigAssTerrainSim

# docker 
sudo docker build -t bats:latest .
sudo docker run --gpus all -dit --mount type=bind,source="$(pwd)",target=/usr/src bats:latest


# copernicus data; this is a great dataset but haven't completely figured it out
key: 807QSJUD3SKJJ0PBY9H4
secret: N8qXCFofGiQdc0GU9SOZKOArgkCmzVsH75TJnsL4
aws configure
export AWS_ENDPOINT_URL=https://eodata.dataspace.copernicus.eu/
aws s3 ls s3://eodata/auxdata/CopDEM_COG/copernicus-dem-30m/Copernicus_DSM_COG_10_S90_00_W180_00_DEM/


# formatting
```bash
sudo apt-get install clang-format
clang-format -style=Google -dump-config > .clang-format
find src include -iname "*.cpp" -o -iname "*.h" | xargs clang-format -i
```
sudo apt-get install clang-tidy
find src -name "*.cpp" | xargs -r -I{} clang-tidy {} -- -Iinclude



# testing
get gtest
```bash
mkdir -p external/gtest && cd external/gtest
git clone https://github.com/google/googletest.git
cd googletest
cmake -DBUILD_GMOCK=OFF -DBUILD_GTEST=ON .
make
```
build tests
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```
ctest --output-on-failure


# documenting with doxygen
`doxygen -g Doxyfile`
This creates Doxyfile, the config file for generating documentation.

3️⃣ Enable C++ Support in Doxyfile
Edit Doxyfile and modify:

```ini
EXTRACT_ALL            = YES
FILE_PATTERNS          = *.cpp *.h
GENERATE_HTML          = YES
GENERATE_LATEX         = NO
```

4️⃣ Generate Documentation
```bash
doxygen Doxyfile
```
This creates an HTML documentation in html/. Open:

```bash
xdg-open html/index.html
```