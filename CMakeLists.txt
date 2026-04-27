cmake_minimum_required(VERSION 3.21)
project(vulkan-path-tracer C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --- Vulkan ---
find_package(Vulkan REQUIRED)

# --- SDL3 (manual, since it's vendored in third_party) ---
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL3")
add_library(SDL3::SDL3 SHARED IMPORTED)
set_target_properties(SDL3::SDL3 PROPERTIES
    IMPORTED_LOCATION             "${SDL3_DIR}/lib/x64/SDL3.dll"
    IMPORTED_IMPLIB               "${SDL3_DIR}/lib/x64/SDL3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL3_DIR}/include"
)

# --- Executable ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")
add_executable(vulkan-path-tracer ${SOURCES})

target_include_directories(vulkan-path-tracer PRIVATE include)
target_link_libraries(vulkan-path-tracer PRIVATE Vulkan::Vulkan SDL3::SDL3)

# Copy SDL3.dll next to the exe at build time
add_custom_command(TARGET vulkan-path-tracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:vulkan-path-tracer>
)

# --- Shader compilation ---
find_program(GLSLC glslc HINTS "$ENV{VULKAN_SDK}/Bin" REQUIRED)

set(SHADER_SRC_DIR "${CMAKE_SOURCE_DIR}/shaders")
set(SHADER_OUT_DIR "${CMAKE_SOURCE_DIR}/shaders/spir-v")
file(MAKE_DIRECTORY "${SHADER_OUT_DIR}")

file(GLOB SHADER_SOURCES CONFIGURE_DEPENDS
    "${SHADER_SRC_DIR}/*.rgen"
    "${SHADER_SRC_DIR}/*.rmiss"
    "${SHADER_SRC_DIR}/*.rchit"
    "${SHADER_SRC_DIR}/*.rahit"
    "${SHADER_SRC_DIR}/*.rint"
    "${SHADER_SRC_DIR}/*.comp"
    "${SHADER_SRC_DIR}/*.vert"
    "${SHADER_SRC_DIR}/*.frag"
)

set(SHADER_BINARIES "")
foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(SPV_OUT "${SHADER_OUT_DIR}/${SHADER_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPV_OUT}
        COMMAND ${GLSLC} --target-env=vulkan1.4 -o ${SPV_OUT} ${SHADER}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${SHADER_NAME}"
        VERBATIM
    )
    list(APPEND SHADER_BINARIES ${SPV_OUT})
endforeach()

add_custom_target(shaders DEPENDS ${SHADER_BINARIES})
add_dependencies(vulkan-path-tracer shaders)