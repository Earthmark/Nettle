#ifndef HOST_H_
#define HOST_H_

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "absl/status/statusor.h"

#include "hostfxr.h"
#include "coreclr_delegates.h"

#include "loader.h"
#include "status_utils.h"

#define HOST_REF Host &h
#define HOST_ACC h
#define HOST_GET_PTR_FN get_func

struct CmdLineInit
{
  std::vector<std::wstring> args;
  std::optional<std::wstring> host_path;
  std::optional<std::wstring> dotnet_root;
};

struct RuntimeCfgInit
{
  std::wstring runtime_cfg_path;
  std::optional<std::wstring> host_path;
  std::optional<std::wstring> dotnet_root;
};

#define FOR_HDT_HANDLES(sub_macro, term)                 \
  sub_macro(load_assembly_and_get_function_pointer) term \
  sub_macro(get_function_pointer)

#define HDTR_FIELD(fn_name) \
  fn_name##_fn fn_name##_

class Host
{
public:
  static absl::StatusOr<Host> init(const CmdLineInit &args);
  static absl::StatusOr<Host> init(const RuntimeCfgInit &args);

  absl::StatusOr<std::wstring_view> get_runtime_property_value(const std::wstring &name);
  absl::Status set_runtime_property_value(const std::wstring &name, const std::wstring &value);
  absl::StatusOr<std::vector<std::pair<std::wstring_view, std::wstring_view>>> get_runtime_properties();

  template <typename T>
  absl::StatusOr<T> get_delegate_func(
      const std::wstring &type_name,
      const std::wstring &method_name,
      const std::wstring &delegate_type_name);
  template <typename T>
  absl::StatusOr<T> get_func(
      const std::wstring &type_name,
      const std::wstring &method_name);

  absl::Status run();
  absl::Status close();

private:
  Host(hostfxr_handle hndl, Loader b);

  static absl::StatusOr<Host> init_internal(hostfxr_handle hndl, Loader b);

  absl::StatusOr<void *> get_runtime_delegate_internal(enum hostfxr_delegate_type type);
  template <typename T>
  absl::StatusOr<T> get_runtime_delegate(enum hostfxr_delegate_type type);

  absl::StatusOr<void *> load_assembly_and_get_function_pointer_internal(
      const std::wstring &assembly_path,
      const std::wstring &type_name,
      const std::wstring &method_name,
      const std::wstring &delegate_type_name);

  absl::StatusOr<void *> get_function_pointer_internal(
      const std::wstring &type_name,
      const std::wstring &method_name,
      const std::wstring *delegate_type_name);

  FOR_HDT_HANDLES(HDTR_FIELD, ;);

  Loader b_;
  hostfxr_handle hndl_;
};

template <typename T>
absl::StatusOr<T> Host::get_runtime_delegate(enum hostfxr_delegate_type type)
{
  ASSIGN_OR_RETURN(void *handle, get_runtime_delegate_internal(type));
  return reinterpret_cast<T>(handle);
}

template <typename T>
absl::StatusOr<T> Host::get_delegate_func(
    const std::wstring &type_name,
    const std::wstring &method_name,
    const std::wstring &delegate_type_name)
{
  ASSIGN_OR_RETURN(void *handle, get_function_pointer_internal(type_name, method_name, &delegate_type_name));
  return reinterpret_cast<T>(handle);
}

template <typename T>
absl::StatusOr<T> Host::get_func(
    const std::wstring &type_name,
    const std::wstring &method_name)
{
  ASSIGN_OR_RETURN(void *handle, get_function_pointer_internal(type_name, method_name, nullptr));
  return reinterpret_cast<T>(handle);
}

#endif
