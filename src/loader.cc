#include "loader.h"

#include <nethost.h>

#define NON_ZERO_ERR(fn, ...)                               \
  if (int32_t status = fn; status)                          \
  {                                                         \
    return absl::InternalError("Error while calling " #fn); \
  }

#define CHECK_HANDLE(name)                                              \
  if (!host.name)                                                       \
  {                                                                     \
    return absl::InternalError("Failed to load native handler " #name); \
  }

#define COMMA ,

absl::StatusOr<const Loader> Loader::init()
{
  size_t len = 1024;
  char_t buffer[1024];
  NON_ZERO_ERR(get_hostfxr_path(buffer, &len, nullptr));
  HINSTANCE dll = LoadLibraryW(buffer);
  Loader host(dll);
  FOR_HOSTFXR_HANDLES(CHECK_HANDLE, ;);
  return host;
}

#define CTOR_INIT(fn_name)                          \
  fn_name(reinterpret_cast<hostfxr_##fn_name##_fn>( \
      GetProcAddress(dll, "hostfxr_" #fn_name)))

Loader::Loader(HINSTANCE dll) : FOR_HOSTFXR_HANDLES(CTOR_INIT, COMMA)
{
}
