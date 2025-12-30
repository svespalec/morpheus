#pragma once

#include <ntddk.h>
#include <intrin.h>

// MSRs
#define MSR_EFER        0xC0000080
#define MSR_VM_CR       0xC0010114
#define MSR_VM_HSAVE_PA 0xC0010117

// EFER bits
#define EFER_SVME ( 1ULL << 12 )

// VM_CR bits
#define VM_CR_SVMDIS ( 1ULL << 4 )

namespace Svm {
  BOOLEAN IsSupported( );
  BOOLEAN IsDisabled( );
  void    Enable( );
} // namespace Svm