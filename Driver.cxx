#include "Svm.hxx"

VOID DriverUnload( PDRIVER_OBJECT DriverObject ) {
  UNREFERENCED_PARAMETER( DriverObject );

  DbgPrint( "Unloaded" );
}

NTSTATUS DriverEntry( PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath ) {
  UNREFERENCED_PARAMETER( RegistryPath );

  DriverObject->DriverUnload = DriverUnload;

  DbgPrint( "Loaded" );

  return STATUS_SUCCESS;
}