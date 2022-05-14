#include "host.h"

#include <iostream>

#include <hostfxr.h>
#include <coreclr_delegates.h>

#define NON_ZERO_ERR(fn, ...)                               \
  if (int32_t status = fn; status)                          \
  {                                                         \
    return absl::InternalError("Error while calling " #fn); \
  }

absl::StatusOr<Host> Host::initialize(const Loader &b, const CmdLineInit &args)
{
  std::vector<const char_t *> a;
  a.resize(args.args.size());
  for (auto arg : args.args)
  {
    a.push_back(arg.data());
  }

  hostfxr_handle hndl;
  NON_ZERO_ERR(b.initialize_for_dotnet_command_line(a.size(), a.data(), nullptr, &hndl));

  return Host(hndl, b);
}

absl::StatusOr<Host> Host::initialize(const Loader &b, const RuntimeCfgInit &args)
{
  hostfxr_handle hndl;
  NON_ZERO_ERR(b.initialize_for_runtime_config(args.runtime_cfg_path.data(), nullptr, &hndl));

  return Host(hndl, b);
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

  std::vector<const char_t *> keys(count);
  std::vector<const char_t *> values(count);
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
