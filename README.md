# Networking
Some networking stuff in c

To compile:

```bash
mkdir build && cd build && cmake .. && cmake --build .
```

To connect to the server via terminal:

```bash
telnet 0.0.0.0 8080
```

To create a docker image:
```bash
sudo docker build -t <your_image_name> .
```

To run image in a docker container:
```bash
runDocker.sh <your_image_name>
```


