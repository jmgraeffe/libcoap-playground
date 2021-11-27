cmake_minimum_required(VERSION 3.12)
project(libcoap-test)

# libcoap
set(ENABLE_TCP OFF CACHE BOOL "Enable building with TCP support")
set(ENABLE_EXAMPLES OFF CACHE BOOL "build also examples")
set(ENABLE_DOCS OFF CACHE BOOL "build also doxygen documentation")
set(ENABLE_DTLS OFF CACHE BOOL "Enable building with DTLS support")
add_subdirectory(libcoap)

# ---------------------

set(COMMON_SRCS common.c)
set(COMMON_HDRS common.h)

# ---------------------

set(CLIENT_SRCS client.c ${COMMON_SRCS})
set(CLIENT_HDRS ${COMMON_HDRS})

add_executable(client ${CLIENT_SRCS} ${CLIENT_HDRS})
target_link_libraries(client PRIVATE coap-3)

# ---------------------

set(SERVER_SRCS server.c ${COMMON_SRCS})
set(SERVER_HDRS ${COMMON_HDRS})

add_executable(server ${SERVER_SRCS} ${SERVER_HDRS})
target_link_libraries(server PRIVATE coap-3)