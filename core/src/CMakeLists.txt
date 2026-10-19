file(
  GLOB_RECURSE
  SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Определение путей к TensorRT
set(TENSORRT_DIR "${CMAKE_SOURCE_DIR}/thirdParty/TensorRT-Enterprise-11.4.0.106-Linux-x86_64-cuda-13.4-Release-external/TensorRT-11.4.0.106")

add_executable(core ${SOURCES})

find_package(OpenCV 4 REQUIRED)
find_package(CUDAToolkit  REQUIRED)

target_include_directories(
  core PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Logger/include

  ${CMAKE_CURRENT_SOURCE_DIR}/DataLayer
  ${CMAKE_CURRENT_SOURCE_DIR}/DataLayer/Data
  ${CMAKE_CURRENT_SOURCE_DIR}/DataLayer/Data/include
  ${CMAKE_CURRENT_SOURCE_DIR}/DataLayer/Queue
  ${CMAKE_CURRENT_SOURCE_DIR}/DataLayer/Queue/include

  ${CMAKE_CURRENT_SOURCE_DIR}/EngineLayer
  ${CMAKE_CURRENT_SOURCE_DIR}/EngineLayer/InferenceEngine/include

  ${CMAKE_CURRENT_SOURCE_DIR}/PipelineLayer
  ${CMAKE_CURRENT_SOURCE_DIR}/PipelineLayer/Pipeline/include

  ${CMAKE_CURRENT_SOURCE_DIR}/LogWriter/include

  ${TENSORRT_DIR}/include
)

target_link_libraries(core PRIVATE
  ${OpenCV_LIBS}
  CUDA::cudart

  ${TENSORRT_DIR}/lib/libnvinfer.so
)

add_custom_command(
  TARGET core
  POST_BUILD
  COMMAND size core --format=gnu
  COMMENT "Размеры секций:"
)

add_custom_command(
  TARGET core
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${CMAKE_BINARY_DIR}/src/core
          ${CMAKE_SOURCE_DIR}/bin/core
  COMMENT "Копирование Core ELF."
)
