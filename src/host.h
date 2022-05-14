#ifndef HOST_H_
#define HOST_H_

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include <hostfxr.h>

#include "bootstrapper.h"

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

class Host
{
public:
  static std::optional<Host> initialize(const Bootstrapper &b, const CmdLineInit &args);
  static std::optional<Host> initialize(const Bootstrapper &b, const RuntimeCfgInit &args);

  std::optional<std::wstring_view> get_runtime_property_value(std::wstring_view name);
  bool set_runtime_property_value(std::wstring_view name, std::wstring_view value);
  std::vector<std::pair<std::wstring_view, std::wstring_view>> get_runtime_properties();

  void run();
  void close();

private:
  Host(hostfxr_handle hndl, const Bootstrapper &b) : hndl_(hndl), b_(b) {}

  const Bootstrapper &b_;
  hostfxr_handle hndl_;
};

#endif