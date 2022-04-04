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

set ( SOURCE_NEXT_WEEK
    ${COMMON_ALL}
    src/common/aabb.h
    src/common/external/stb_image.h
    src/common/perlin.h
    src/common/rtw_stb_image.h
    src/common/texture.h
    src/RTTheNextWeek/aarect.h
    src/RTTheNextWeek/box.h
    src/RTTheNextWeek/bvh.h
    src/RTTheNextWeek/constant_medium.h
    src/RTTheNextWeek/hittable.h
    src/RTTheNextWeek/hittable_list.h
    src/RTTheNextWeek/material.h
    src/RTTheNextWeek/moving_sphere.h
    src/RTTheNextWeek/sphere.h
    src/RTTheNextWeek/main.cc
)

set ( SOURCE_REST_LIFE
    ${COMMON_ALL}
    src/common/aabb.h
    src/common/external/stb_image.h
    src/common/perlin.h
    src/common/rtw_stb_image.h
    src/common/texture.h
    src/RTTheRestOfLife/aarect.h
    src/RTTheRestOfLife/box.h
    src/RTTheRestOfLife/bvh.h
    src/RTTheRestOfLife/hittable.h
    src/RTTheRestOfLife/hittable_list.h
    src/RTTheRestOfLife/material.h
    src/RTTheRestOfLife/onb.h
    src/RTTheRestOfLife/pdf.h
    src/RTTheRestOfLife/sphere.h
    src/RTTheRestOfLife/main.cc
)

# Executables
add_executable(RTInOneWeekend       ${SOURCE_ONE_WEEKEND})
add_executable(RTTheNextWeek        ${SOURCE_NEXT_WEEK})
add_executable(RTTheRestOfLife      ${SOURCE_REST_LIFE})
add_executable(cos_cubed            src/RTTheRestOfLife/cos_cubed.cc   ${COMMON_ALL})
add_executable(cos_density          src/RTTheRestOfLife/cos_density.cc   ${COMMON_ALL})
add_executable(integrate_x_sq       src/RTTheRestOfLife/integrate_x_sq.cc   ${COMMON_ALL})
add_executable(pi                   src/RTTheRestOfLife/pi.cc   ${COMMON_ALL})

include_directories(src/common)