include(FetchContent)

# googltest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# abseil
FetchContent_Declare(
  absl-cpp
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG        215105818dfde3174fe799600bb0f3cae233d0bf # 20211102.0
)
set(ABSL_PROPAGATE_CXX_STD ON)
FetchContent_MakeAvailable(absl-cpp)

# nethost
set(azure_feed "https://dotnetcli.azureedge.net/dotnet")
set(channel "LTS")
set(runtime_version "6.0.5")
set(specific_version "6.0.105")
set(specific_product_version "6.0.105")
set(osname "win")
set(normalized_architecture "x64")

FetchContent_Declare(
  dotnet
  URL       "${azure_feed}/Sdk/${specific_version}/dotnet-sdk-${specific_product_version}-${osname}-${normalized_architecture}.zip"
  URL_HASH  SHA512=8407515bd7fb9b3a5f9a3abd33e2d282b90d49a904a28434ef7c90a4be8f8c45aa87b98d4b261503c7121ba299eeb500032a69d52fa961fb39319b661156ba31
)
FetchContent_GetProperties(dotnet)
FetchContent_Populate(dotnet)

set(DOTNET_NATIVE_PATH "${dotnet_SOURCE_DIR}/packs/Microsoft.NETCore.App.Host.${osname}-${normalized_architecture}/${runtime_version}/runtimes/${osname}-${normalized_architecture}/native")
add_library(nethost SHARED IMPORTED GLOBAL)
target_include_directories(nethost INTERFACE "${DOTNET_NATIVE_PATH}")
set_property(TARGET nethost PROPERTY IMPORTED_IMPLIB ${DOTNET_NATIVE_PATH}/nethost.lib)
set_property(TARGET nethost PROPERTY IMPORTED_LOCATION ${DOTNET_NATIVE_PATH}/nethost.dll)
