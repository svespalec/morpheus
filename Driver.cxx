#include "Serial.hxx"
#include "Svm.hxx"

VOID DriverUnload( PDRIVER_OBJECT DriverObject ) {
  UNREFERENCED_PARAMETER( DriverObject );

  DbgPrint( "Unloaded" );
}

NTSTATUS DriverEntry( PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath ) {
  UNREFERENCED_PARAMETER( RegistryPath );

  DriverObject->DriverUnload = DriverUnload;

  DbgPrint( "Loaded" );

  Serial::PrintHex( "CR0", __readcr0() );
  Serial::PrintHex( "CR3", __readcr3() );
  Serial::PrintHex( "CR4", __readcr4() );

  return STATUS_SUCCESS;
}