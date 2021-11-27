# libcoap playground

## Setup
```
sudo apt install build-essential cmake
git submodule update --init --recursive

mkdir cmake_build && cd cmake_build
cmake ..
```

## Compile

```
cd cmake_build
make
```

## Run

```
cd cmake_build
./server
```

```
cd cmake_build
./client
```