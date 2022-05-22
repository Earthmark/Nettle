#include "interop_back.h"

namespace Managed
{
  int Add(int a, int b)
  {
    return a + b;
  }
  void AddList(absl::Span<const int> a, absl::Span<const int> b, absl::Span<int> o)
  {
    for (int i = 0; i < a.length(); ++i)
    {
      o[i] = a[i] + b[i];
    }
  }
} // Managed
