cmake_minimum_required ( VERSION 3.1.0 )

project (RTWeekend
    VERSION 3.0.0
    LANGUAGES CXX
)

# Set to c++11
set ( CMAKE_CXX_STANDARD 11 )

# Source
set ( COMMON_ALL
    src/common/rtweekend.h
    src/common/camera.h
    src/common/ray.h
    src/common/vec3.h
)

set ( SOURCE_ONE_WEEKEND
    ${COMMON_ALL}
    src/RTInOneWeekend/hittable.h
    src/RTInOneWeekend/hittable_list.h
    src/RTInOneWeekend/material.h
    src/RTInOneWeekend/sphere.h
    src/RTInOneWeekend/main.cc
)

# Executables
add_executable(RTInOneWeekend       ${SOURCE_ONE_WEEKEND})

include_directories(src/common)