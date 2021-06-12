cmake_minimum_required(VERSION 3.5)
#set(CMAKE_BUILD_TYPE "Debug") #定义编译类型
#set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb") # 定义debug 模式

set(CMAKE_SYSTEM_NAME Linux)
if(TARGET_ARCH_ABI STREQUAL "armv8")
    set(CMAKE_SYSTEM_PROCESSOR aarch64)
    set(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
    set(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")
elseif(TARGET_ARCH_ABI STREQUAL "armv7hf")
    set(CMAKE_SYSTEM_PROCESSOR arm)
    set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
    set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
elseif(TARGET_ARCH_ABI STREQUAL "himix100")
    # set cross-compiled system type, it's better not use the type which cmake cannot recognized.
    SET ( CMAKE_SYSTEM_PROCESSOR aarch64 )
    # when hislicon SDK was installed, toolchain was installed in the path as below: 
    SET ( CMAKE_C_COMPILER "/opt/hisi-linux/x86-arm/aarch64-himix100-linux/bin/aarch64-himix100-linux-gcc" )
    SET ( CMAKE_CXX_COMPILER "/opt/hisi-linux/x86-arm/aarch64-himix100-linux/bin/aarch64-himix100-linux-g++" )
    SET ( CMAKE_FIND_ROOT_PATH "/opt/hisi-linux/x86-arm/aarch64-himix100-linux" )

    # set searching rules for cross-compiler
    #SET ( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
    #SET ( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
    #SET ( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )

    # set ${CMAKE_C_FLAGS} and ${CMAKE_CXX_FLAGS}flag for cross-compiled process
    SET ( CMAKE_CXX_FLAGS "-march=armv8-a ${CMAKE_CXX_FLAGS}" )

    # cache flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "c flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "c++ flags")
elseif(TARGET_ARCH_ABI STREQUAL "himix200")
    # set cross-compiled system type, it's better not use the type which cmake cannot recognized.
    SET ( CMAKE_SYSTEM_PROCESSOR arm )
    # when hislicon SDK was installed, toolchain was installed in the path as below: 
    SET ( CMAKE_C_COMPILER /opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-gcc )
    SET ( CMAKE_CXX_COMPILER /opt/hisi-linux/x86-arm/arm-himix200-linux/bin/arm-himix200-linux-g++ )

    # set searching rules for cross-compiler
    SET ( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
    SET ( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
    SET ( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )

    # set ${CMAKE_C_FLAGS} and ${CMAKE_CXX_FLAGS}flag for cross-compiled process
    SET ( CROSS_COMPILATION_ARM himix200 )
    SET ( CROSS_COMPILATION_ARCHITECTURE armv7-a )

    # set g++ param
    SET ( CMAKE_CXX_FLAGS "-march=armv7-a -mfloat-abi=softfp -mfpu=neon-vfpv4 ${CMAKE_CXX_FLAGS}" )

    # cache flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "c flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "c++ flags")
else()
    message(FATAL_ERROR "Unknown arch abi ${TARGET_ARCH_ABI}, only support armv8 and armv7hf.")
    return()
endif()

project(object_detection_demo)
message(STATUS "TARGET ARCH ABI: ${TARGET_ARCH_ABI}")

# 配置OpenMP
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/../arm_env/jsoncpp-1.9.4/include)
link_directories( ${CMAKE_CURRENT_LIST_DIR}/../arm_env/jsoncpp-1.9.4/lib)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../arm_env/qdian/include)
link_directories( ${CMAKE_CURRENT_LIST_DIR}/../arm_env/qdian/lib)


link_directories( ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if(TARGET_ARCH_ABI STREQUAL "armv8")
    set(CMAKE_CXX_FLAGS "-march=armv8-a ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "-march=armv8-a ${CMAKE_C_FLAGS}")
elseif(TARGET_ARCH_ABI STREQUAL "armv7hf")
    set(CMAKE_CXX_FLAGS "-march=armv7-a -mfloat-abi=hard -mfpu=neon-vfpv4 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "-march=armv7-a -mfloat-abi=hard -mfpu=neon-vfpv4 ${CMAKE_C_FLAGS}" )
endif()

set(OpenCV_DIR ${CMAKE_CURRENT_LIST_DIR}/../arm_env/opencv-3.4.1/share)
message(STATUS "OpenCV path:${OpenCV_DIR}")

find_package(OpenCV REQUIRED PATHS ${OpenCV_DIR} NO_DEFAULT_PATH)

if(OpenCV_FOUND OR OpenCV_CXX_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "OpenCV library status:")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Could not found OpenCV!")
    return()
endif()
add_executable(detect test_detect.cpp utils.cpp)
target_link_libraries(detect  detetor jsoncpp  ${OpenCV_LIBS})

