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
} // namespace Serial