file(
  GLOB_RECURSE
  SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(core ${SOURCES})

# Опции.
option(USE_HAILO "Отладка." ON)

if (USE_HAILO)
  target_compile_definitions(core PRIVATE USE_HAILO)
endif()

find_package(OpenCV 4 REQUIRED)

if (USE_HAILO)
  find_library(HAILORT_LIB hailort REQUIRED)
endif()

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

  ${CMAKE_SOURCE_DIR}/Utilities/TimerManager/include

  ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/hailort-5.4.0/hailort/libhailort/include/hailo
)

target_link_libraries(core PRIVATE
  ${OpenCV_LIBS}
  ${HAILORT_LIB}
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
