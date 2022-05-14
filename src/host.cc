#include "host.h"

#include <iostream>

#include <hostfxr.h>
#include <coreclr_delegates.h>

std::optional<Host> Host::initialize(const Bootstrapper &b, const CmdLineInit &args)
{
  std::vector<const char_t *> a;
  a.resize(args.args.size());
  for (auto arg : args.args)
  {
    a.push_back(arg.data());
  }

  hostfxr_handle handle;
  if (!b.initialize_for_dotnet_command_line(a.size(), a.data(), nullptr, &handle))
  {
    return std::nullopt;
  }

  return Host(handle, b);
}

std::optional<Host> Host::initialize(const Bootstrapper &b, const RuntimeCfgInit &args)
{
  hostfxr_handle handle;
  if (auto status = b.initialize_for_runtime_config(args.runtime_cfg_path.data(), nullptr, &handle); status)
  {
    std::cout << "Error loading host: " << status << std::endl;
    return std::nullopt;
  }

  return Host(handle, b);
}

std::optional<std::wstring_view> Host::get_runtime_property_value(std::wstring_view name)
{
  const char_t *value;
  if (!b_.get_runtime_property_value(hndl_, name.data(), &value))
  {
    return std::nullopt;
  }
  return value;
}

bool Host::set_runtime_property_value(std::wstring_view name, std::wstring_view value)
{
  return !b_.set_runtime_property_value(hndl_, name.data(), value.data());
}

std::vector<std::pair<std::wstring_view, std::wstring_view>> Host::get_runtime_properties()
{
  size_t count = 0;
  auto status = b_.get_runtime_properties(hndl_, &count, nullptr, nullptr);

  std::cout << "Properties available: " << count << std::endl;

  std::vector<const char_t *> keys(count);
  std::vector<const char_t *> values(count);

  status = b_.get_runtime_properties(hndl_, &count, keys.data(), values.data());
  if (!status)
  {
    std::cout << "Failed to get runtime properties" << std::endl;
  }

  std::vector<std::pair<std::wstring_view, std::wstring_view>> pairs;
  pairs.reserve(count);
  for (size_t index = 0; index < count; ++index)
  {
    pairs.push_back({keys[index], values[index]});
  }

  return pairs;
}

void Host::run()
{
  if (auto status = b_.run_app(hndl_); status)
  {
    std::cout << "Error starting " << status << std::endl;
  }
}

void Host::close()
{
  b_.close(hndl_);
}
