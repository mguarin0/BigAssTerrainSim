# Use Ubuntu as the base image
FROM pytorch/pytorch:2.6.0-cuda11.8-cudnn9-devel

# Set non-interactive mode for apt
#ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    clang \
    clang-tools \
    clang-format \
    cmake \
    libgtest-dev \
    lldb \
    gdb \
    g++ \
    make \
    ninja-build \
    doxygen \
    libgdal-dev \
    gdal-bin \
    libthrust-dev \
    git \
    wget \
    curl \
    && apt-get clean && rm -rf /var/lib/apt/lists/*
RUN apt install nvidia-cuda-toolkit
# doing this for eu copernicus datasets
# RUN apt-get install unzip groff less -y
# RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
# RUN unzip awscliv2.zip
# RUN ./aws/install

# cpp deps

# Verify installation
# RUN clang --version && python --version && cmake --version && g++ --version

# Set up a working directory
WORKDIR /usr/src

# TODO update thrust to use https://github.com/NVIDIA/cccl