#include "Svm.hxx"

BOOLEAN Svm::IsSupported( ) {
  int cpuInfo[ 4 ];

  // Check AMD vendor
  __cpuid( cpuInfo, 0 );

  // Check for AuthenticAMD
  if ( cpuInfo[ 1 ] != 'htuA' || cpuInfo[ 3 ] != 'itne' || cpuInfo[ 2 ] != 'DMAc' ) {
    Serial::Print( "[SVM] Not AMD CPU\n" );
    return FALSE;
  }

  // Check SVM feature bit
  __cpuid( cpuInfo, 0x80000001 );

  if ( ( cpuInfo[ 2 ] & ( 1 << 2 ) ) == 0 ) {
    Serial::Print( "[SVM] SVM not supported\n" );
    return FALSE;
  }

  Serial::Print( "[SVM] SVM supported\n" );

  return TRUE;
}

BOOLEAN Svm::IsDisabled( ) {
  UINT64 vmCr = __readmsr( MSR_VM_CR );

  if ( vmCr & VM_CR_SVMDIS ) {
    Serial::Print( "[SVM] SVM disabled in BIOS\n" );
    return TRUE;
  }

  return FALSE;
}

void Svm::Enable( ) {
  UINT64 efer  = __readmsr( MSR_EFER );
  efer        |= EFER_SVME;

  __writemsr( MSR_EFER, efer );

  Serial::Print( "[SVM] EFER.SVME enabled\n" );
}