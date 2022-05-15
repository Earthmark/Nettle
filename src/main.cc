#include <iostream>
#include <string>

#include "absl/status/statusor.h"

#include "host.h"
#include "status_utils.h"

absl::StatusOr<int> status_main()
{
  CmdLineInit args{
      .args = {
          L"C:\\Users\\earth\\source\\repos\\nettle\\managed\\bin\\Debug\\net6.0\\nettle.dll",
      },
  };
  ASSIGN_OR_RETURN(Host host, Host::init(args));
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
