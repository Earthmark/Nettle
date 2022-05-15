cmake_minimum_required(VERSION 3.22)
project (Nettle)

file(GLOB NETTLE_SRC
  "src/*.cc"
  "src/*.h"
)

add_executable (Nettle ${NETTLE_SRC})
set_property(TARGET Nettle PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  absl-cpp
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG        215105818dfde3174fe799600bb0f3cae233d0bf # 20211102.0
)
set(ABSL_PROPAGATE_CXX_STD ON)
FetchContent_MakeAvailable(absl-cpp)

add_subdirectory(third_party/nethost)

target_link_libraries(Nettle nethost absl::base absl::status absl::statusor absl::strings)

add_custom_command(TARGET Nettle POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Nettle> $<TARGET_FILE_DIR:Nettle>
  COMMAND_EXPAND_LISTS
)
