#include "Serial.hxx"
#include "Svm.hxx"

VOID DriverUnload( PDRIVER_OBJECT DriverObject ) {
  UNREFERENCED_PARAMETER( DriverObject );
}

NTSTATUS DriverEntry( PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath ) {
  UNREFERENCED_PARAMETER( RegistryPath );

  DriverObject->DriverUnload = DriverUnload;

  Serial::Init();

  Serial::Print( "[HV] Driver loading\n" );

  if ( !Svm::IsSupported() || Svm::IsDisabled() )
    return STATUS_NOT_SUPPORTED;

  Serial::Print( "[HV] All checks passed\n" );

  Serial::PrintHex( "CR0", __readcr0() );
  Serial::PrintHex( "CR3", __readcr3() );
  Serial::PrintHex( "CR4", __readcr4() );

  return STATUS_SUCCESS;
}