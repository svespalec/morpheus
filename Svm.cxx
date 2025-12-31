#include "Svm.hxx"

// Checks if CPU even has SVM hardware capability (AMD + SVM feature bit)
// This will return true even if SVM is disabled in BIOS as it only checks capabilities
BOOLEAN Svm::IsSupported() {
  int cpuInfo[ 4 ];

  // Check AMD vendor
  __cpuid( cpuInfo, 0 );

  // Check for AuthenticAMD
  if ( cpuInfo[ 1 ] != 'htuA' || cpuInfo[ 3 ] != 'itne' || cpuInfo[ 2 ] != 'DMAc' ) {
    LOG( "Not AMD CPU" );
    return FALSE;
  }

  // Check SVM feature bit
  __cpuid( cpuInfo, 0x80000001 );

  if ( ( cpuInfo[ 2 ] & ( 1 << 2 ) ) == 0 ) {
    LOG( "SVM not supported" );
    return FALSE;
  }

  LOG( "SVM supported" );

  return TRUE;
}

// Checks if BIOS has forcibly locked out SVM (VM_CR.SVMDIS bit)
// This is different from SVM just not being enabled.
// It means the BIOS explicitly prevents SVM usage which is common
// on enterprise machines where IT locks down virtualization.
BOOLEAN Svm::IsDisabled() {
  UINT64 vmCr = __readmsr( MSR_VM_CR );

  if ( vmCr & VM_CR_SVMDIS ) {
    LOG( "SVM disabled in BIOS" );
    return TRUE;
  }

  return FALSE;
}

void Svm::Enable() {
  // EFER (Extended Feature Enable Register) controls CPU features like SVM, long mode, NX, etc.
  // It's basically just a bunch of on/off switches for extended CPU functionality.
  // The BIOS enables SVM capability, but we must set EFER.SVME to actually use it pretty much
  UINT64 efer = __readmsr( MSR_EFER );
  efer |= EFER_SVME;

  __writemsr( MSR_EFER, efer );

  LOG( "EFER.SVME enabled" );
}