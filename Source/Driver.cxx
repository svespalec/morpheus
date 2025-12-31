#include "Serial.hxx"
#include "Svm.hxx"

VOID DriverUnload( PDRIVER_OBJECT DriverObject ) {
  UNREFERENCED_PARAMETER( DriverObject );
}

NTSTATUS DriverEntry( PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath ) {
  UNREFERENCED_PARAMETER( RegistryPath );

  DriverObject->DriverUnload = DriverUnload;

  Serial::Init();

  LOG( "Driver loading" );

  if ( !Svm::IsSupported() || Svm::IsDisabled() )
    return STATUS_NOT_SUPPORTED;

  LOG( "All checks passed" );

  LOG_HEX( "CR0", __readcr0() );
  LOG_HEX( "CR3", __readcr3() );
  LOG_HEX( "CR4", __readcr4() );

  return STATUS_SUCCESS;
}