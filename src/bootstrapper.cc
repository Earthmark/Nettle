#include "bootstrapper.h"

#include <nethost.h>

#define VALID_HANDLE(name) \
  host.name

#define COMMA ,

std::optional<const Bootstrapper> Bootstrapper::init()
{
  size_t len = 1024;
  char_t buffer[1024];
  if (get_hostfxr_path(buffer, &len, nullptr))
  {
    return std::nullopt;
  }
  HINSTANCE dll = LoadLibraryW(buffer);
  Bootstrapper host(dll);
  if (FOR_HANDLES(VALID_HANDLE, &&))
  {
    return host;
  }
  return std::nullopt;
}

#define CTOR_INIT(fn_name)                          \
  fn_name(reinterpret_cast<hostfxr_##fn_name##_fn>( \
      GetProcAddress(dll, "hostfxr_" #fn_name)))

Bootstrapper::Bootstrapper(HINSTANCE dll) : FOR_HANDLES(CTOR_INIT, COMMA)
{
}
