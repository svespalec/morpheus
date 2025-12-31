#include <intrin.h>
#include <Windows.h>
#include <print>

#define LOG( fmt, ... ) std::println( fmt, ##__VA_ARGS__ )

int main() {
  int cpuInfo[ 4 ];

  // Check hypervisor present bit first
  __cpuid( cpuInfo, 1 );

  bool hvPresent = ( cpuInfo[ 2 ] >> 31 ) & 1;

  LOG( "[!] Hypervisor present: {}", hvPresent ? "YES" : "NO" );

  if ( !hvPresent ) {
    LOG( "[-] No hypervisor detected" );
    LOG( "[!] Press ENTER to exit!" );

    return std::getchar();
  }

  // Get hypervisor signature (leaf 0x40000000)
  __cpuid( cpuInfo, 0x40000000 );

  char hvVendor[ 13 ] = {};

  std::memcpy( &hvVendor[ 0 ], &cpuInfo[ 1 ], 4 ); // EBX
  std::memcpy( &hvVendor[ 4 ], &cpuInfo[ 2 ], 4 ); // ECX
  std::memcpy( &hvVendor[ 8 ], &cpuInfo[ 3 ], 4 ); // EDX

  LOG( "[!] Hypervisor vendor: {}", hvVendor );

  if ( std::strcmp( hvVendor, "Morpheus" ) == 0 )
    LOG( "[+] Running under our hypervisor!" );

  LOG( "[!] Press ENTER to exit!" );

  return std::getchar();
}