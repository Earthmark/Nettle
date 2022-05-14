cmake_minimum_required(VERSION 3.22)
project (Nettle)

file(GLOB Nettle_Src
     "src/*.cc"
)
add_executable (Nettle ${Nettle_Src})

set_property(TARGET Nettle PROPERTY CXX_STANDARD 20)

set(DOTNET_NATIVE_PATH "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/6.0.5/runtimes/win-x64/native")

add_library(nethost SHARED IMPORTED GLOBAL)
set_property(TARGET nethost PROPERTY IMPORTED_IMPLIB "${DOTNET_NATIVE_PATH}/nethost.lib")
set_property(TARGET nethost PROPERTY IMPORTED_LOCATION "${DOTNET_NATIVE_PATH}/nethost.dll")

include_directories(SYSTEM "${DOTNET_NATIVE_PATH}")

target_link_libraries(Nettle nethost)

message(STATUS ${CMAKE_CURRENT_BINARY_DIR} )
message(STATUS ${DOTNET_NATIVE_PATH}/nethost.dll )
add_custom_command(TARGET Nettle POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Nettle> $<TARGET_FILE_DIR:Nettle>
  COMMAND_EXPAND_LISTS
  )
