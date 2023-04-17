cmake_minimum_required(VERSION 3.25)
project(chess)
if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()
set(CMAKE_CXX_STANDARD 20)

include_directories(chess)
include_directories(tests)