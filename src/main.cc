#include <Windows.h>
#include <hostfxr.h>
#include <nethost.h>

#include <iostream>

#include "host.h"
#include "bootstrapper.h"

int main()
{
  auto bs = Bootstrapper::init();
  if (!bs)
  {
    std::cout << "Failed to load hostfxr" << std::endl;
    return 1;
  }

  RuntimeCfgInit init;
  init.runtime_cfg_path = L"C:\\Users\\earth\\source\\repos\\nettle\\managed\\bin\\Debug\\net6.0\\nettle.runtimeconfig.json";
  auto host = Host::initialize(*bs, init);
  if (!host)
  {
    std::cout << "Failed to load host" << std::endl;
    return 1;
  }

  std::wcout << L"Host props" << std::endl;
  for (auto [k, v] : host->get_runtime_properties())
  {
    std::wcout << L"  " << k << L"=" << v << std::endl;
  }

  host->run();

  return 0;
}