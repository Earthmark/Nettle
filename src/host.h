#ifndef HOST_H_
#define HOST_H_

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "absl/status/statusor.h"

#include "hostfxr.h"

#include "loader.h"

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
  static absl::StatusOr<Host> initialize(const Loader &b, const CmdLineInit &args);
  static absl::StatusOr<Host> initialize(const Loader &b, const RuntimeCfgInit &args);

  absl::StatusOr<std::wstring_view> get_runtime_property_value(std::wstring_view name);
  absl::Status set_runtime_property_value(std::wstring_view name, std::wstring_view value);
  absl::StatusOr<std::vector<std::pair<std::wstring_view, std::wstring_view>>> get_runtime_properties();

  absl::Status run();
  absl::Status close();

private:
  Host(hostfxr_handle hndl, const Loader &b) : hndl_(hndl), b_(b) {}

  const Loader &b_;
  hostfxr_handle hndl_;
};

#endif