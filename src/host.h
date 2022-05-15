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

struct CmdLineInit
{
  std::vector<std::wstring_view> args;
  std::optional<std::wstring_view> host_path;
  std::optional<std::wstring_view> dotnet_root;
};

struct RuntimeCfgInit
{
  std::wstring_view runtime_cfg_path;
  std::optional<std::wstring_view> host_path;
  std::optional<std::wstring_view> dotnet_root;
};

#define FOR_HDT_HANDLES(sub_macro, term)                 \
  sub_macro(load_assembly_and_get_function_pointer) term \
  sub_macro(get_function_pointer)

#define HDTR_HANDLE(fn_name) \
  fn_name##_fn fn_name

class Host
{
public:
  static absl::StatusOr<Host> init(const CmdLineInit &args);
  static absl::StatusOr<Host> init(const RuntimeCfgInit &args);

  absl::StatusOr<std::wstring_view> get_runtime_property_value(std::wstring_view name);
  absl::Status set_runtime_property_value(std::wstring_view name, std::wstring_view value);
  absl::StatusOr<std::vector<std::pair<std::wstring_view, std::wstring_view>>> get_runtime_properties();

  absl::Status run();
  absl::Status close();

private:
  Host(hostfxr_handle hndl, Loader b);

  absl::StatusOr<void *> get_runtime_delegate_internal(enum hostfxr_delegate_type type);
  template <typename T>
  absl::StatusOr<T> get_runtime_delegate(enum hostfxr_delegate_type type);

  absl::StatusOr<void *> load_assembly_and_get_function_pointer_internal(
      std::wstring_view assembly_path,
      std::wstring_view type_name,
      std::wstring_view method_name,
      std::wstring_view delegate_type_name);

  absl::StatusOr<void *> get_function_pointer_internal(
      std::wstring_view type_name,
      std::wstring_view method_name,
      std::wstring_view delegate_type_name);

  FOR_HDT_HANDLES(HDTR_HANDLE, ;);

  Loader b_;
  hostfxr_handle hndl_;
};

template <typename T>
absl::StatusOr<T> Host::get_runtime_delegate(enum hostfxr_delegate_type type)
{
  ASSIGN_OR_RETURN(void *handle, get_runtime_delegate_internal(type));
  return reinterpret_cast<T>(handle);
}

#endif
