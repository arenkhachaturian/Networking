# Use a base image with necessary dependencies
FROM ubuntu:latest

# Set the working directory inside the container
WORKDIR /app

# Install necessary dependencies
RUN apt-get update && \
    apt-get install -y wget build-essential libssl-dev cmake

# Copy the source code into the container
COPY . /app

# Build your project using CMake
RUN cd build && cmake .. && cmake --build .

RUN chmod +x /app/run.sh

# Expose any ports your application listens on
EXPOSE $PORT_NUM

ENTRYPOINT ["/app/run.sh"]

