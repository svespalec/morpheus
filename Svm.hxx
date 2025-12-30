#pragma once

#include <ntddk.h>
#include <intrin.h>

namespace Svm {
  BOOLEAN IsSupported( );
  BOOLEAN IsDisabled( );
  void    Enable( );
} // namespace Svm