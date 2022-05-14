#ifndef BOOTSTRAPPER_H_
#define BOOTSTRAPPER_H_

#include <optional>
#include <string_view>

#include <Windows.h>
#include <hostfxr.h>

#define FOR_HANDLES(sub_macro, term)            \
  sub_macro(set_error_writer) term              \
  sub_macro(initialize_for_dotnet_command_line) \
  term                                          \
  sub_macro(initialize_for_runtime_config)      \
  term                                          \
  sub_macro(get_runtime_property_value)         \
  term                                          \
  sub_macro(set_runtime_property_value)         \
  term                                          \
  sub_macro(get_runtime_properties)             \
  term                                          \
  sub_macro(run_app)                            \
  term                                          \
  sub_macro(get_runtime_delegate)               \
  term                                          \
  sub_macro(close)

#define HOSTFXR_HANDLE(fn_name) \
  hostfxr_##fn_name##_fn fn_name

class Bootstrapper
{
public:
  static std::optional<const Bootstrapper> init();

  FOR_HANDLES(HOSTFXR_HANDLE, ;);

private:
  Bootstrapper(HINSTANCE dll);
};

#endif