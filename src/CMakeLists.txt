file(
  GLOB_RECURSE
  SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(ProjectX ${SOURCES})

target_include_directories(
  ProjectX PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/EngineLayer/include
  ${CMAKE_CURRENT_SOURCE_DIR}/LlmEngine/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Pipeline/include
)

target_link_libraries(
  ProjectX PRIVATE
)

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
