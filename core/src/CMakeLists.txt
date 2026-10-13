file(
  GLOB_RECURSE
  SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(core ${SOURCES})

find_package(OpenCV 4 REQUIRED)

target_include_directories(
  core PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Logger/include
  ${CMAKE_CURRENT_SOURCE_DIR}/EngineLayer
  ${CMAKE_CURRENT_SOURCE_DIR}/EngineLayer/InferenceEngine/include
  ${CMAKE_CURRENT_SOURCE_DIR}/EngineLayer/DataWriterEngine/include
  ${CMAKE_CURRENT_SOURCE_DIR}/PipelineLayer
  ${CMAKE_CURRENT_SOURCE_DIR}/PipelineLayer/Pipeline/include
  ${CMAKE_CURRENT_SOURCE_DIR}/DataLayer/ThreadQueue
  ${CMAKE_CURRENT_SOURCE_DIR}/DataLayer/ThreadQueue/include
)

target_link_libraries(core PUBLIC ${OpenCV_LIBS})

add_custom_command(
  TARGET core
  POST_BUILD
  COMMAND size core --format=gnu
  COMMENT "Размеры секций:"
)


add_custom_command(
  TARGET core
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_BINARY_DIR}/src
          ${CMAKE_SOURCE_DIR}/bin
  COMMENT "Копирование исполняемого файла."
)
