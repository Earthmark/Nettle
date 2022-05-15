#include "status_utils.h"

#include "absl/strings/cord.h"
#include "absl/strings/str_format.h"

absl::Status InvalidArgOnNonZero(int32_t status_code, std::string_view message)
{
  if (status_code)
  {
    absl::Status status = absl::InvalidArgumentError(message);
    status.SetPayload("status code", absl::Cord(absl::StrFormat("0x%X", status_code)));
    return status;
  }
  return absl::OkStatus();
}
