#include <iostream>
#include <string>

#include "absl/status/statusor.h"

#include "host.h"
#include "status_utils.h"
#include "interop_back.h"
#include "gen.h"

absl::StatusOr<int> do_stuff(Host &host)
{
  ASSIGN_OR_RETURN(
      ManagedCodespace::NativeInterface inter,
      ManagedCodespace::NativeInterface::init_NativeInterface(host));

  int a = inter.DoSimpleManagedThings(1, 2);

  ManagedCodespace::NativeObject obj{
      .a = a,
      .callback = [](int i)
      {
        std::cout << i << std::endl;
      }};
  return inter.DoManagedThings(&obj, 4);
}

absl::StatusOr<int> status_main()
{
  CmdLineInit args{
      .args = {
          L"C:\\Users\\earth\\source\\repos\\nettle\\managed\\bin\\Debug\\net6.0\\nettle.dll",
      },
  };
  ASSIGN_OR_RETURN(Host host, Host::init(args));

  RETURN_IF_ERROR(Managed::Interop_Install(host));

  RETURN_IF_ERROR(do_stuff(host));

  RETURN_IF_ERROR(host.run());
  return 0;
}

int main()
{
  if (auto status = status_main(); !status.ok())
  {
    std::cout << status.status() << std::endl;
    return 1;
  }
  else
  {
    return *status;
  }
}
