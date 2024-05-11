# Use a base image with necessary dependencies
FROM ubuntu:latest

# Set the working directory inside the container
WORKDIR /app

# Install necessary dependencies
RUN apt-get update && \
    apt-get install -y wget build-essential libssl-dev

# Download and install CMake from source
RUN wget https://github.com/Kitware/CMake/releases/download/v3.22.1/cmake-3.22.1.tar.gz && \
    tar -xzvf cmake-3.22.1.tar.gz && \
    cd cmake-3.22.1 && \
    ./bootstrap && \
    make -j$(nproc) && \
    make install && \
    cd .. && \
    rm -rf cmake-3.22.1 cmake-3.22.1.tar.gz

# Copy the source code into the container
COPY . /app

ENV USER_DATA_FILE /app/data/users.txt

# Build your project using CMake
RUN cd build && cmake .. && cmake --build .

# Expose any ports your application listens on
EXPOSE 8080

# Define the command to run your application
CMD ["./build/server"]
