#include "interop_back.h"

#include "managed_interop.h"
#include "status_utils.h"

namespace Managed {
  absl::Status Interop_Install(Host& host) {
    ASSIGN_OR_RETURN(Interop interface_obj, Interop::init_Interop(host));

    Interop_InteropImpl impl {
      .Add = [](int a, int b) {
          return Add(a, b);
      },
      .AddList = [](InteropSpan<int> a, InteropSpan<int>b, InteropSpan<int>o){
          AddList(a, b, o);
      },
    };

    interface_obj.PopulateInterop(&impl);

    return absl::OkStatus();
  }
} // Managed
