#ifndef MANAGED_INTEROP_BACK_H
#define MANAGED_INTEROP_BACK_H

#include "Host.h"

namespace Managed {
  absl::Status Interop_Install(Host& host);

  int Add(int a, int b);
  void AddList(absl::Span<const int> a, absl::Span<const int> b, absl::Span<int> o);

} // Managed

#endif // MANAGED_INTEROP_BACK_H
