# Use Ubuntu as the base image
FROM pytorch/pytorch:2.6.0-cuda11.8-cudnn9-devel

# Set non-interactive mode for apt
#ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies
# RUN apt-get update && apt-get install -y --no-install-recommends \
#     clang \
#     clang-tools \
#     cmake \
#     libgtest-dev \
#     lldb \
#     gdb \
#     g++ \
#     make \
#     ninja-build \
#     python3 \
#     python3-pip \
#     python3-dev \
#     python3-venv \
#     git \
#     wget \
#     curl \
#     && apt-get clean && rm -rf /var/lib/apt/lists/*


# doing this for eu copernicus datasets
# RUN apt-get install unzip groff less -y
# RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
# RUN unzip awscliv2.zip
# RUN ./aws/install

# cpp deps
#RUN apt-get install libgdal-dev gdal-bin -y

# Verify installation
# RUN clang --version && python --version && cmake --version && g++ --version

# Set up a working directory
WORKDIR /usr/src

