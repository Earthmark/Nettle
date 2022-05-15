#ifndef EIDOS_PORT_STATUS_MACROS_H_
#define EIDOS_PORT_STATUS_MACROS_H_

#include "absl/base/optimization.h"
#include "absl/status/status.h"

absl::Status InvalidArgOnNonZero(int32_t status_code, std::string_view message);

#define STATUS_ERROR_NON_ZERO(fn) \
  RETURN_IF_ERROR(InvalidArgOnNonZero(fn, "Error while calling " #fn))

#define STATUS_MACROS_CONCAT_NAME(x, y) \
  STATUS_MACROS_CONCAT_IMPL(x, y)
#define STATUS_MACROS_CONCAT_IMPL(x, y) x##y

#define ASSIGN_OR_RETURN(lhs, rexpr)                                 \
  ASSIGN_OR_RETURN_IMPL(                                             \
      STATUS_MACROS_CONCAT_NAME(_status_or_value, __COUNTER__), lhs, \
      rexpr)

#define ASSIGN_OR_RETURN_IMPL(statusor, lhs, rexpr) \
  const auto statusor = (rexpr);                    \
  if (ABSL_PREDICT_FALSE(!statusor.ok()))           \
  {                                                 \
    return statusor.status();                       \
  }                                                 \
  lhs = std::move(statusor.value());

#define RETURN_IF_ERROR_IMPL(__local_status, __status) \
  const auto __local_status = __status;                \
  if (ABSL_PREDICT_FALSE(!__local_status.ok()))        \
  {                                                    \
    return __local_status;                             \
  }

#define RETURN_IF_ERROR(status) \
  RETURN_IF_ERROR_IMPL(         \
      STATUS_MACROS_CONCAT_NAME(_local_status, __COUNTER__), status)

#define RETURN_IF_NULL(ptr)                                            \
  if (!(ptr))                                                          \
  {                                                                    \
    return absl::InvalidArgumentError(absl::StrCat(#ptr, " is null")); \
  }

// We need dependency on logging to import the core CHECK macros.
#if !defined(CHECK_OK)
#define CHECK_OK(value) CHECK((value).ok())
#endif // CHECK_OK

#endif // EIDOS_PORT_STATUS_MACROS_H_