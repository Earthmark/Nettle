#ifndef GEN_H_
#define GEN_H_

#include "coreclr_delegates.h"

// This include may be different depending on implementation.
// In this case this header exports Host, which provides access to a CLR environment.
#include "host.h"

namespace ManagedCodespace
{
  struct NativeObject
  {
    int a;
    void(__cdecl *callback)(int);
  };

  class NativeInterface
  {
  public:
    static absl::StatusOr<NativeInterface> init_NativeInterface(Host &host);

    /// <summary>
    /// Does basic math stuff.
    /// </summary>
    int DoSimpleManagedThings(int a, int b);

    int DoManagedThings(NativeObject *obj, int b);

  private:
    typedef int(__cdecl *DoSimpleManagedThings_fn)(int a, int b);
    DoSimpleManagedThings_fn DoSimpleManagedThings_;

    typedef int(__cdecl *DoManagedThings_fn)(NativeObject *obj, int b);
    DoManagedThings_fn DoManagedThings_;

    NativeInterface();
  };
} // namespace ManagedCodespace

#endif