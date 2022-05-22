#include "gen.h"

#include "status_utils.h"

namespace ManagedCodespace
{

#define ASSEMBLY L"ManagedCodespace.NativeInterface, nettle"

    absl::StatusOr<NativeInterface> NativeInterface::init_NativeInterface(Host &host)
    {
        NativeInterface interface_obj;

        ASSIGN_OR_RETURN(
            interface_obj.DoSimpleManagedThings_,
            host.get_func<DoSimpleManagedThings_fn>(
                ASSEMBLY,
                L"DoSimpleManagedThings"));

        ASSIGN_OR_RETURN(
            interface_obj.DoManagedThings_,
            host.get_func<DoManagedThings_fn>(
                ASSEMBLY,
                L"DoManagedThings"));

        return interface_obj;
    }

    int NativeInterface::DoSimpleManagedThings(int a, int b)
    {
        return DoSimpleManagedThings_(a, b);
    }

    int NativeInterface::DoManagedThings(NativeObject *obj, int b)
    {
        return DoManagedThings_(obj, b);
    }

    NativeInterface::NativeInterface()
    {
    }
} // namespace ManagedCodespace
