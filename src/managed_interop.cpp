#include "managed_interop.h"

#include "status_utils.h"

namespace Managed {
  absl::StatusOr<Interop> Interop::init_Interop(Host& host) {
    Interop interface_obj;

    ASSIGN_OR_RETURN(
      interface_obj.PopulateInterop_,
      host.get_func<PopulateInterop_fn>(
        L"Managed.Interop, nettle",
        L"PopulateInterop"));

    return interface_obj;
  }

  void Interop::PopulateInterop(Interop_InteropImpl *impl) {
    PopulateInterop_(impl);
  }

}  // namespace Managed
