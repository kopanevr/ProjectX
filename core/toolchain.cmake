set(
  CMAKE_CXX_FLAGS
  " \
  -fno-exceptions \
  -Wno-deprecated \
  "
)

add_compile_definitions(ORT_NO_EXCEPTIONS)
