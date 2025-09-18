FROM ubuntu:24.04
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y build-essential cmake ninja-build
RUN apt-get update && apt-get install -y \
    build-essential \
    clang-format \
    cmake \
    git \
    gnome-desktop-testing \
    libasound2-dev \
    libaudio-dev \
    libdbus-1-dev \
    libdecor-0-dev \
    libdrm-dev \
    libegl1-mesa-dev \
    libfreetype-dev \
    libgbm-dev \
    libgl1-mesa-dev \
    libgles2-mesa-dev \
    libharfbuzz-dev \
    libibus-1.0-dev \
    libjack-dev \
    libpipewire-0.3-dev \
    libpulse-dev \
    libsndio-dev \
    libudev-dev \
    liburing-dev \
    libwayland-dev \
    libx11-dev \
    libxcursor-dev \
    libxext-dev \
    libxfixes-dev \
    libxi-dev \
    libxkbcommon-dev \
    libxrandr-dev \
    libxss-dev \
    libxtst-dev \
    make \
    ninja-build \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*
