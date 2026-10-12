file(
  GLOB_RECURSE
  SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(ProjectX ${SOURCES})

find_package(OpenCV 4 REQUIRED)

target_include_directories(
  ProjectX PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Logger/include
  ${CMAKE_CURRENT_SOURCE_DIR}/EngineLayer
  ${CMAKE_CURRENT_SOURCE_DIR}/EngineLayer/YoloEngine/include
  ${CMAKE_CURRENT_SOURCE_DIR}/PipelineLayer
  ${CMAKE_CURRENT_SOURCE_DIR}/PipelineLayer/Pipeline/include
  ${CMAKE_CURRENT_SOURCE_DIR}/DataLayer/ThreadQueue
  ${CMAKE_CURRENT_SOURCE_DIR}/DataLayer/ThreadQueue/include
)

target_link_libraries(ProjectX PUBLIC ${OpenCV_LIBS})

add_custom_command(
  TARGET ProjectX
  POST_BUILD
  COMMAND size ProjectX --format=gnu
  COMMENT "Размеры секций:"
)

add_custom_command(
  TARGET ProjectX
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
          ${CMAKE_SOURCE_DIR}/models
          ${CMAKE_BINARY_DIR}/models
  COMMENT "Копирование директории моделей в ${CMAKE_BINARY_DIR}."
)
