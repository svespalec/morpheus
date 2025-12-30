#pragma once

#include <ntddk.h>

namespace Svm {
  BOOLEAN IsSupported( );
  BOOLEAN IsDisabled( );
  void    Enable( );
} // namespace Svm