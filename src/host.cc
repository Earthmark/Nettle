#include "host.h"

#include <iostream>

#include <hostfxr.h>
#include <coreclr_delegates.h>

#define NON_ZERO_ERR(fn)                                    \
  if (int32_t status = fn; status)                          \
  {                                                         \
    return absl::InternalError("Error while calling " #fn); \
  }

#define INIT_HDT_HANDLE(fn_name) \
  ASSIGN_OR_RETURN(host.fn_name, host.get_runtime_delegate<fn_name##_fn>(hdt_##fn_name));

absl::StatusOr<Host> Host::init(const CmdLineInit &args)
{
  ASSIGN_OR_RETURN(Loader b, Loader::init());

  std::vector<const char_t *> a;
  a.reserve(args.args.size());
  for (auto arg : args.args)
  {
    a.push_back(arg.data());
  }

  hostfxr_handle hndl;
  NON_ZERO_ERR(b.initialize_for_dotnet_command_line(a.size(), a.data(), nullptr, &hndl));

  Host host(hndl, std::move(b));
  FOR_HDT_HANDLES(INIT_HDT_HANDLE, ;);

  return host;
}

absl::StatusOr<Host> Host::init(const RuntimeCfgInit &args)
{
  ASSIGN_OR_RETURN(Loader b, Loader::init());

  hostfxr_handle hndl;
  NON_ZERO_ERR(b.initialize_for_runtime_config(args.runtime_cfg_path.data(), nullptr, &hndl));

  Host host(hndl, std::move(b));
  FOR_HDT_HANDLES(INIT_HDT_HANDLE, ;);

  return host;
}

absl::StatusOr<std::wstring_view> Host::get_runtime_property_value(std::wstring_view name)
{
  const char_t *value;
  NON_ZERO_ERR(b_.get_runtime_property_value(hndl_, name.data(), &value));
  return value;
}

absl::Status Host::set_runtime_property_value(std::wstring_view name, std::wstring_view value)
{
  NON_ZERO_ERR(b_.set_runtime_property_value(hndl_, name.data(), value.data()));
  return absl::OkStatus();
}

absl::StatusOr<std::vector<std::pair<std::wstring_view, std::wstring_view>>> Host::get_runtime_properties()
{
  size_t count = 0;
  b_.get_runtime_properties(hndl_, &count, nullptr, nullptr);

  std::vector<const char_t *> keys;
  keys.resize(count);
  std::vector<const char_t *> values;
  values.resize(count);
  NON_ZERO_ERR(b_.get_runtime_properties(hndl_, &count, keys.data(), values.data()));

  std::vector<std::pair<std::wstring_view, std::wstring_view>> pairs;
  pairs.reserve(count);
  for (size_t index = 0; index < count; ++index)
  {
    pairs.push_back({keys[index], values[index]});
  }

  return pairs;
}

absl::Status Host::run()
{
  NON_ZERO_ERR(b_.run_app(hndl_));
  return absl::OkStatus();
}

absl::Status Host::close()
{
  NON_ZERO_ERR(b_.close(hndl_));
  return absl::OkStatus();
}

Host::Host(hostfxr_handle hndl, Loader b) : hndl_(hndl), b_(std::move(b))
{
}

absl::StatusOr<void *> Host::get_runtime_delegate_internal(enum hostfxr_delegate_type type)
{
  void *handle;
  NON_ZERO_ERR(b_.get_runtime_delegate(hndl_, type, &handle));
  return handle;
}

absl::StatusOr<void *> Host::load_assembly_and_get_function_pointer_internal(
    std::wstring_view assembly_path,
    std::wstring_view type_name,
    std::wstring_view method_name,
    std::wstring_view delegate_type_name)
{
  void *ptr;
  NON_ZERO_ERR(load_assembly_and_get_function_pointer(
      assembly_path.data(), type_name.data(), method_name.data(), delegate_type_name.data(),
      nullptr, &ptr));
  return ptr;
}

absl::StatusOr<void *> Host::get_function_pointer_internal(
    std::wstring_view type_name,
    std::wstring_view method_name,
    std::wstring_view delegate_type_name)
{
  void *ptr;
  NON_ZERO_ERR(get_function_pointer(
      type_name.data(), method_name.data(), delegate_type_name.data(),
      nullptr, nullptr, &ptr));
  return ptr;
}
