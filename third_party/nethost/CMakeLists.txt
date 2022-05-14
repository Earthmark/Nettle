cmake_minimum_required(VERSION 3.22)
project (nethost)

set(DOTNET_NATIVE_PATH "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/6.0.5/runtimes/win-x64/native")

add_library(nethost SHARED IMPORTED GLOBAL)
target_include_directories(nethost INTERFACE "${DOTNET_NATIVE_PATH}")
set_property(TARGET nethost PROPERTY IMPORTED_IMPLIB ${DOTNET_NATIVE_PATH}/nethost.lib)
set_property(TARGET nethost PROPERTY IMPORTED_LOCATION ${DOTNET_NATIVE_PATH}/nethost.dll)
