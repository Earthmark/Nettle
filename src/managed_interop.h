#ifndef NETTLE_MANAGED_INTEROP_H_
#define NETTLE_MANAGED_INTEROP_H_

#include "host.h"

namespace Managed {

    template<typename T>
    struct InteropSpan {
        T* Data;
        int Length;

        operator absl::Span<T>() {
            return absl::Span(Data, Length);
        }
        operator absl::Span<const T>() {
            return absl::Span(Data, Length);
        }
    };

    struct Interop_InteropImpl {
        int (__cdecl *Add)(int a, int b);
        void (__cdecl *AddList)(InteropSpan<int> a, InteropSpan<int> b, InteropSpan<int> o);
    };

    class Interop {
    public:
        static absl::StatusOr<Interop> init_Interop(Host& host);

        void PopulateInterop(Interop_InteropImpl* impl);

    private:
        typedef int(__cdecl *PopulateInterop_fn)(Interop_InteropImpl* impl);
        PopulateInterop_fn PopulateInterop_;

        Interop() {};
    };
}  // namespace Managed

#endif //NETTLE_MANAGED_INTEROP_H_
