#include <iostream>

#include <Windows.h>
#include <hostfxr.h>
#include <nethost.h>

#include "absl/status/statusor.h"

#include "host.h"
#include "loader.h"
#include "status_macros.h"

absl::StatusOr<int> status_main()
{
  ASSIGN_OR_RETURN(const Loader loader, Loader::init());
  RuntimeCfgInit init{
      .runtime_cfg_path = L"C:\\Users\\earth\\source\\repos\\nettle\\managed\\bin\\Debug\\net6.0\\nettle.runtimeconfig.json",
  };
  ASSIGN_OR_RETURN(Host host, Host::initialize(loader, init));

  ASSIGN_OR_RETURN(auto props, host.get_runtime_properties());
  for (auto [k, v] : props)
  {
    std::wcout << L"  " << k << L"=" << v << std::endl;
  }

  RETURN_IF_ERROR(host.run());

  return 0;
}

int main()
{
  if (auto status = status_main(); !status.ok())
  {
    std::cout << status.status() << std::endl;
  }
  else
  {
    return *status;
  }
}
