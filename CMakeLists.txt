cmake_minimum_required(VERSION 3.16)
project(ext4-example)

option(WITHOUT_TEST "Disable unit test" OFF)
set(CMAKE_CXX_STANDARD 17)

add_library(ext4 STATIC
    src/ext4/crc32.cpp
    src/ext4/superblock.cpp
    src/ext4/filesystem.cpp
    src/ext4/filesystems.cpp
    src/ext4/core.cpp
    src/ext4/bytearray_reader.cpp
    src/ext4/blockgroup_descriptor.cpp
    src/ext4/inode.cpp
    src/ext4/block.cpp
    src/ext4/filemode.cpp
)
target_include_directories(ext4 PUBLIC include)
target_include_directories(ext4 PRIVATE src)

add_executable(ext4-ls src/app/ext4_ls.cpp)
target_link_libraries(ext4-ls PRIVATE ext4)

add_executable(ext4-info src/app/ext4_info.cpp)
target_link_libraries(ext4-info PRIVATE ext4)


if(NOT(WITHOUT_TEST))
enable_testing()
include(CTest)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(alltests
    test-src/test_crc32.cpp
    test-src/test_time.cpp
)
target_link_libraries(alltests PRIVATE ext4 GTest::gtest GTest::gtest_main)
target_include_directories(alltests PRIVATE src)
gtest_discover_tests(alltests)

endif()
