#include "loader.h"

#define NETHOST_USE_AS_STATIC

#include "nethost.h"

#include "status_utils.h"

#define CHECK_HANDLE(name)                                                     \
  if (!host.name)                                                              \
  {                                                                            \
    return absl::InvalidArgumentError("Failed to load native handler " #name); \
  }

absl::StatusOr<const Loader> Loader::init()
{
  constexpr size_t BUFF_LEN = 1024;
  size_t len = BUFF_LEN;
  char_t buffer[BUFF_LEN];
  STATUS_ERROR_NON_ZERO(get_hostfxr_path(buffer, &len, nullptr));
  HINSTANCE dll = LoadLibraryW(buffer);
  Loader host(dll);
  FOR_HOSTFXR_HANDLES(CHECK_HANDLE, ;);
  return host;
}

#define CTOR_INIT(fn_name)                          \
  fn_name(reinterpret_cast<hostfxr_##fn_name##_fn>( \
      GetProcAddress(dll, "hostfxr_" #fn_name)))

#define COMMA ,

Loader::Loader(HINSTANCE dll) : FOR_HOSTFXR_HANDLES(CTOR_INIT, COMMA)
{
}
