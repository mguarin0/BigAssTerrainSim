# BigAssTerrainSim

# docker 
sudo docker build -t ubuntu-clang-python .
sudo docker run -dit --mount type=bind,source="$(pwd)",target=/usr/src ubuntu-clang-python


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
```


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