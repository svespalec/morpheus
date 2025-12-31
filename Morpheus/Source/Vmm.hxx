#pragma once

#include <intrin.h>
#include <ntddk.h>

#include "Svm.hxx"
#include "Serial.hxx"

namespace Vmm {
  BOOLEAN IsSvmSupported();
  BOOLEAN IsSvmDisabled();
  void EnableSvm();
} // namespace Svm