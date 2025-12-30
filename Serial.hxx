#pragma once

#include <ntddk.h>

namespace Serial {
  constexpr UINT16 COM1 = 0x3F8;

  inline void Init( ) {
    __outbyte( COM1 + 1, 0x00 ); // Disable interrupts
    __outbyte( COM1 + 3, 0x80 ); // Enable DLAB
    __outbyte( COM1 + 0, 0x01 ); // 115200 baud
    __outbyte( COM1 + 1, 0x00 );
    __outbyte( COM1 + 3, 0x03 ); // 8N1
    __outbyte( COM1 + 2, 0xC7 ); // Enable FIFO
    __outbyte( COM1 + 4, 0x0B ); // RTS/DSR set
  }

  inline void PutChar( char c ) {
    while ( ( __inbyte( COM1 + 5 ) & 0x20 ) == 0 )
      ;

    __outbyte( COM1, c );
  }

  inline void Print( const char* str ) {
    for ( ; *str; ++str ) {
      if ( *str == '\n' )
        PutChar( '\r' );

      PutChar( *str );
    }
  }

  inline void PrintHex( const char* label, UINT64 val ) {
    Print( label );
    Print( ": 0x" );

    char buf[ 17 ];

    for ( int i = 15; i >= 0; i-- ) {
      int nibble    = ( val >> ( i * 4 ) ) & 0xF;
      buf[ 15 - i ] = nibble < 10 ? '0' + nibble : 'A' + nibble - 10;
    }

    buf[ 16 ] = 0;

    Print( buf );
    Print( "\n" );
  }
} // namespace Serial