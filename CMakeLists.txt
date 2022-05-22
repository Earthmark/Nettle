cmake_minimum_required(VERSION 3.22)
project (Nettle)

file(GLOB NETTLE_SRC
  "src/*.cc"
  "src/*.h"
)

add_executable (Nettle ${NETTLE_SRC} src/managed_interop.cpp src/managed_interop.h src/interop_back.cpp src/interop_back.h)
set_property(TARGET Nettle PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party)

target_link_libraries(Nettle nethost absl::base absl::status absl::statusor absl::strings)

add_custom_command(TARGET Nettle POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Nettle> $<TARGET_FILE_DIR:Nettle>
  COMMAND_EXPAND_LISTS
)
