#include "host.h"

#include <iostream>

#include "hostfxr.h"
#include "coreclr_delegates.h"

#include "status_utils.h"

absl::StatusOr<Host> Host::init(const CmdLineInit &args)
{
  ASSIGN_OR_RETURN(Loader b, Loader::init());

  std::vector<const char_t *> native_buffer;
  native_buffer.reserve(args.args.size());
  for (const auto &arg : args.args)
  {
    native_buffer.push_back(arg.c_str());
  }

  hostfxr_handle hndl;
  STATUS_ERROR_NON_ZERO(b.initialize_for_dotnet_command_line(native_buffer.size(), native_buffer.data(), nullptr, &hndl));

  return init_internal(hndl, std::move(b));
}

absl::StatusOr<Host> Host::init(const RuntimeCfgInit &args)
{
  ASSIGN_OR_RETURN(Loader b, Loader::init());

  hostfxr_handle hndl;
  STATUS_ERROR_NON_ZERO(b.initialize_for_runtime_config(args.runtime_cfg_path.c_str(), nullptr, &hndl));

  return init_internal(hndl, std::move(b));
}

#define INIT_HDT_HANDLE(fn_name) \
  ASSIGN_OR_RETURN(host.fn_name, host.get_runtime_delegate<fn_name##_fn>(hdt_##fn_name));

absl::StatusOr<Host> Host::init_internal(hostfxr_handle hndl, Loader b)
{
  Host host(hndl, std::move(b));
  FOR_HDT_HANDLES(INIT_HDT_HANDLE, ;);

  return host;
}

Host::Host(hostfxr_handle hndl, Loader b) : hndl_(hndl), b_(std::move(b))
{
}

absl::StatusOr<std::wstring_view> Host::get_runtime_property_value(const std::wstring &name)
{
  std::wstring solid_name(name);
  const char_t *value;
  STATUS_ERROR_NON_ZERO(b_.get_runtime_property_value(hndl_, solid_name.c_str(), &value));
  return value;
}

absl::Status Host::set_runtime_property_value(const std::wstring &name, const std::wstring &value)
{
  std::wstring solid_name(name);
  std::wstring solid_value(value);
  STATUS_ERROR_NON_ZERO(b_.set_runtime_property_value(hndl_, solid_name.c_str(), solid_value.c_str()));
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
  STATUS_ERROR_NON_ZERO(b_.get_runtime_properties(hndl_, &count, keys.data(), values.data()));

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
  STATUS_ERROR_NON_ZERO(b_.run_app(hndl_));
  return absl::OkStatus();
}

absl::Status Host::close()
{
  STATUS_ERROR_NON_ZERO(b_.close(hndl_));
  return absl::OkStatus();
}

absl::StatusOr<void *> Host::get_runtime_delegate_internal(enum hostfxr_delegate_type type)
{
  void *handle;
  STATUS_ERROR_NON_ZERO(b_.get_runtime_delegate(hndl_, type, &handle));
  return handle;
}

absl::StatusOr<void *> Host::load_assembly_and_get_function_pointer_internal(
    const std::wstring &assembly_path,
    const std::wstring &type_name,
    const std::wstring &method_name,
    const std::wstring &delegate_type_name)
{
  void *ptr;
  STATUS_ERROR_NON_ZERO(load_assembly_and_get_function_pointer(
      assembly_path.c_str(), type_name.c_str(), method_name.c_str(), delegate_type_name.c_str(),
      nullptr, &ptr));
  return ptr;
}

absl::StatusOr<void *> Host::get_function_pointer_internal(
    const std::wstring &type_name,
    const std::wstring &method_name,
    const std::wstring &delegate_type_name)
{
  void *ptr;
  STATUS_ERROR_NON_ZERO(get_function_pointer(
      type_name.c_str(), method_name.c_str(), delegate_type_name.c_str(),
      nullptr, nullptr, &ptr));
  return ptr;
}
