# libcoap playground

Place to play around with libcoap for me, but it's a pretty solid minimal example
for libcoap too I guess.

See branches for different variants.

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
