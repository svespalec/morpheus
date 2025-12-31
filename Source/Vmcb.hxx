#pragma once

#include <ntddk.h>

#pragma pack( push, 1 )

// Based on AMD APM Vol 2, Appendix B and Linux kernel svm.h ( https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/svm.h )

// Segment descriptor (16 bytes)
typedef struct _SEGMENT_DESCRIPTOR {
  UINT16 Selector;
  UINT16 Attributes;
  UINT32 Limit;
  UINT64 Base;
} SEGMENT_DESCRIPTOR, *PSEGMENT_DESCRIPTOR;

// VMCB Control Area (offset 0x000, size 0x400)
typedef struct _VMCB_CONTROL {
  // Intercept vectors (0x000 - 0x017)
  UINT32 InterceptCr;               // 0x000 - CR read/write intercepts (word 0)
  UINT32 InterceptDr;               // 0x004 - DR read/write intercepts (word 1)
  UINT32 InterceptExceptions;       // 0x008 - Exception intercepts (word 2)
  UINT32 InterceptMisc1;            // 0x00C - Misc intercepts (word 3)
  UINT32 InterceptMisc2;            // 0x010 - Misc intercepts (word 4)
  UINT32 InterceptMisc3;            // 0x014 - Misc intercepts (word 5)
  UINT8 Reserved1[ 0x03C - 0x018 ]; // 0x018

  UINT16 PauseFilterThreshold; // 0x03C
  UINT16 PauseFilterCount;     // 0x03E
  UINT64 IopmBasePa;           // 0x040 - Physical address of IOPM
  UINT64 MsrpmBasePa;          // 0x048 - Physical address of MSRPM
  UINT64 TscOffset;            // 0x050
  UINT32 GuestAsid;            // 0x058
  UINT8 TlbControl;            // 0x05C
  UINT8 Reserved2[ 3 ];        // 0x05D

  // Virtual interrupt control (0x060)
  UINT32 VIntControl;   // 0x060
  UINT32 VIntVector;    // 0x064
  UINT32 IntState;      // 0x068 - Interrupt shadow
  UINT8 Reserved3[ 4 ]; // 0x06C

  UINT32 ExitCode;                  // 0x070
  UINT32 ExitCodeHi;                // 0x074
  UINT64 ExitInfo1;                 // 0x078
  UINT64 ExitInfo2;                 // 0x080
  UINT32 ExitIntInfo;               // 0x088
  UINT32 ExitIntInfoErr;            // 0x08C
  UINT64 NestedCtl;                 // 0x090 - Nested paging control (bit 0 = NP enable)
  UINT64 AvicApicBar;               // 0x098
  UINT64 GhcbGpa;                   // 0x0A0
  UINT32 EventInj;                  // 0x0A8
  UINT32 EventInjErr;               // 0x0AC
  UINT64 NestedCr3;                 // 0x0B0 - Nested page table CR3
  UINT64 VirtExt;                   // 0x0B8 - LBR virtualization enable
  UINT32 VmcbClean;                 // 0x0C0
  UINT32 Reserved4;                 // 0x0C4
  UINT64 NextRip;                   // 0x0C8
  UINT8 InsnLen;                    // 0x0D0
  UINT8 InsnBytes[ 15 ];            // 0x0D1
  UINT64 AvicBackingPage;           // 0x0E0
  UINT8 Reserved5[ 8 ];             // 0x0E8
  UINT64 AvicLogicalId;             // 0x0F0
  UINT64 AvicPhysicalId;            // 0x0F8
  UINT8 Reserved6[ 8 ];             // 0x100
  UINT64 VmsaPtr;                   // 0x108 - Used for SEV-ES
  UINT8 Reserved7[ 0x120 - 0x110 ]; // 0x110
  UINT16 BusLockCounter;            // 0x120
  UINT8 Reserved8[ 0x138 - 0x122 ]; // 0x122
  UINT64 AllowedSevFeatures;        // 0x138
  UINT64 GuestSevFeatures;          // 0x140
  UINT8 Reserved9[ 0x3E0 - 0x148 ]; // 0x148
  UINT8 ReservedSw[ 32 ];           // 0x3E0 - Reserved for hypervisor use
} VMCB_CONTROL, *PVMCB_CONTROL;

// CR intercept bits (InterceptCr, word 0)
#define INTERCEPT_CR0_READ  ( 1UL << 0 )
#define INTERCEPT_CR3_READ  ( 1UL << 3 )
#define INTERCEPT_CR4_READ  ( 1UL << 4 )
#define INTERCEPT_CR8_READ  ( 1UL << 8 )
#define INTERCEPT_CR0_WRITE ( 1UL << 16 )
#define INTERCEPT_CR3_WRITE ( 1UL << 19 )
#define INTERCEPT_CR4_WRITE ( 1UL << 20 )
#define INTERCEPT_CR8_WRITE ( 1UL << 24 )

// DR intercept bits (InterceptDr, word 1)
#define INTERCEPT_DR0_READ  ( 1UL << 0 )
#define INTERCEPT_DR7_READ  ( 1UL << 7 )
#define INTERCEPT_DR0_WRITE ( 1UL << 16 )
#define INTERCEPT_DR7_WRITE ( 1UL << 23 )

// Intercept bits for InterceptMisc1 (word 3, offset 0x00C)
#define INTERCEPT_INTR          ( 1UL << 0 )
#define INTERCEPT_NMI           ( 1UL << 1 )
#define INTERCEPT_SMI           ( 1UL << 2 )
#define INTERCEPT_INIT          ( 1UL << 3 )
#define INTERCEPT_VINTR         ( 1UL << 4 )
#define INTERCEPT_SELECTIVE_CR0 ( 1UL << 5 )
#define INTERCEPT_STORE_IDTR    ( 1UL << 6 )
#define INTERCEPT_STORE_GDTR    ( 1UL << 7 )
#define INTERCEPT_STORE_LDTR    ( 1UL << 8 )
#define INTERCEPT_STORE_TR      ( 1UL << 9 )
#define INTERCEPT_LOAD_IDTR     ( 1UL << 10 )
#define INTERCEPT_LOAD_GDTR     ( 1UL << 11 )
#define INTERCEPT_LOAD_LDTR     ( 1UL << 12 )
#define INTERCEPT_LOAD_TR       ( 1UL << 13 )
#define INTERCEPT_RDTSC         ( 1UL << 14 )
#define INTERCEPT_RDPMC         ( 1UL << 15 )
#define INTERCEPT_PUSHF         ( 1UL << 16 )
#define INTERCEPT_POPF          ( 1UL << 17 )
#define INTERCEPT_CPUID         ( 1UL << 18 )
#define INTERCEPT_RSM           ( 1UL << 19 )
#define INTERCEPT_IRET          ( 1UL << 20 )
#define INTERCEPT_INT           ( 1UL << 21 )
#define INTERCEPT_INVD          ( 1UL << 22 )
#define INTERCEPT_PAUSE         ( 1UL << 23 )
#define INTERCEPT_HLT           ( 1UL << 24 )
#define INTERCEPT_INVLPG        ( 1UL << 25 )
#define INTERCEPT_INVLPGA       ( 1UL << 26 )
#define INTERCEPT_IOIO          ( 1UL << 27 )
#define INTERCEPT_MSR           ( 1UL << 28 )
#define INTERCEPT_TASK_SWITCH   ( 1UL << 29 )
#define INTERCEPT_FERR_FREEZE   ( 1UL << 30 )
#define INTERCEPT_SHUTDOWN      ( 1UL << 31 )

// Intercept bits for InterceptMisc2 (word 4, offset 0x010)
#define INTERCEPT_VMRUN           ( 1UL << 0 )
#define INTERCEPT_VMMCALL         ( 1UL << 1 )
#define INTERCEPT_VMLOAD          ( 1UL << 2 )
#define INTERCEPT_VMSAVE          ( 1UL << 3 )
#define INTERCEPT_STGI            ( 1UL << 4 )
#define INTERCEPT_CLGI            ( 1UL << 5 )
#define INTERCEPT_SKINIT          ( 1UL << 6 )
#define INTERCEPT_RDTSCP          ( 1UL << 7 )
#define INTERCEPT_ICEBP           ( 1UL << 8 )
#define INTERCEPT_WBINVD          ( 1UL << 9 )
#define INTERCEPT_MONITOR         ( 1UL << 10 )
#define INTERCEPT_MWAIT           ( 1UL << 11 )
#define INTERCEPT_XSETBV          ( 1UL << 13 )
#define INTERCEPT_RDPRU           ( 1UL << 14 )
#define INTERCEPT_EFER_WRITE_TRAP ( 1UL << 15 )
#define INTERCEPT_CR0_WRITE_TRAP  ( 1UL << 16 )
// CR1-CR8 traps at bits 17-24

// TLB control values
#define TLB_CONTROL_DO_NOTHING       0
#define TLB_CONTROL_FLUSH_ALL_ASID   1
#define TLB_CONTROL_FLUSH_ASID       3
#define TLB_CONTROL_FLUSH_ASID_LOCAL 7

// Virtual interrupt control bits
#define V_TPR_MASK           0x0F
#define V_IRQ_SHIFT          8
#define V_IRQ_MASK           ( 1UL << V_IRQ_SHIFT )
#define V_GIF_SHIFT          9
#define V_GIF_MASK           ( 1UL << V_GIF_SHIFT )
#define V_INTR_PRIO_SHIFT    16
#define V_INTR_PRIO_MASK     ( 0x0FUL << V_INTR_PRIO_SHIFT )
#define V_IGN_TPR_SHIFT      20
#define V_IGN_TPR_MASK       ( 1UL << V_IGN_TPR_SHIFT )
#define V_INTR_MASKING_SHIFT 24
#define V_INTR_MASKING_MASK  ( 1UL << V_INTR_MASKING_SHIFT )
#define V_GIF_ENABLE_SHIFT   25
#define V_GIF_ENABLE_MASK    ( 1UL << V_GIF_ENABLE_SHIFT )

// Nested control bits
#define SVM_NESTED_CTL_NP_ENABLE ( 1ULL << 0 )

// Interrupt shadow
#define SVM_INTERRUPT_SHADOW_MASK ( 1ULL << 0 )

// Event injection
#define SVM_EVTINJ_VEC_MASK   0xFF
#define SVM_EVTINJ_TYPE_SHIFT 8
#define SVM_EVTINJ_TYPE_MASK  ( 7UL << SVM_EVTINJ_TYPE_SHIFT )
#define SVM_EVTINJ_TYPE_INTR  ( 0UL << SVM_EVTINJ_TYPE_SHIFT )
#define SVM_EVTINJ_TYPE_NMI   ( 2UL << SVM_EVTINJ_TYPE_SHIFT )
#define SVM_EVTINJ_TYPE_EXEPT ( 3UL << SVM_EVTINJ_TYPE_SHIFT )
#define SVM_EVTINJ_TYPE_SOFT  ( 4UL << SVM_EVTINJ_TYPE_SHIFT )
#define SVM_EVTINJ_VALID      ( 1UL << 31 )
#define SVM_EVTINJ_VALID_ERR  ( 1UL << 11 )

// VMCB State Save Area (offset 0x400)
// Offsets in comments are relative to VMCB base (add 0x400 to get absolute)
typedef struct _VMCB_STATE_SAVE {
  SEGMENT_DESCRIPTOR Es;            // 0x400
  SEGMENT_DESCRIPTOR Cs;            // 0x410
  SEGMENT_DESCRIPTOR Ss;            // 0x420
  SEGMENT_DESCRIPTOR Ds;            // 0x430
  SEGMENT_DESCRIPTOR Fs;            // 0x440
  SEGMENT_DESCRIPTOR Gs;            // 0x450
  SEGMENT_DESCRIPTOR Gdtr;          // 0x460
  SEGMENT_DESCRIPTOR Ldtr;          // 0x470
  SEGMENT_DESCRIPTOR Idtr;          // 0x480
  SEGMENT_DESCRIPTOR Tr;            // 0x490
  UINT8 Reserved1[ 0x0CB - 0x0A0 ]; // 0x4A0 (43 bytes)
  UINT8 Cpl;                        // 0x4CB
  UINT8 Reserved2[ 4 ];             // 0x4CC
  UINT64 Efer;                      // 0x4D0
  UINT8 Reserved3[ 0x148 - 0x0D8 ]; // 0x4D8 (112 bytes)
  UINT64 Cr4;                       // 0x548
  UINT64 Cr3;                       // 0x550
  UINT64 Cr0;                       // 0x558
  UINT64 Dr7;                       // 0x560
  UINT64 Dr6;                       // 0x568
  UINT64 Rflags;                    // 0x570
  UINT64 Rip;                       // 0x578
  UINT8 Reserved4[ 0x1D8 - 0x180 ]; // 0x580 (88 bytes)
  UINT64 Rsp;                       // 0x5D8
  UINT64 SCet;                      // 0x5E0
  UINT64 Ssp;                       // 0x5E8
  UINT64 IsstAddr;                  // 0x5F0
  UINT64 Rax;                       // 0x5F8
  UINT64 Star;                      // 0x600
  UINT64 LStar;                     // 0x608
  UINT64 CStar;                     // 0x610
  UINT64 SfMask;                    // 0x618
  UINT64 KernelGsBase;              // 0x620
  UINT64 SysenterCs;                // 0x628
  UINT64 SysenterEsp;               // 0x630
  UINT64 SysenterEip;               // 0x638
  UINT64 Cr2;                       // 0x640
  UINT8 Reserved5[ 0x268 - 0x248 ]; // 0x648 (32 bytes)
  UINT64 GPat;                      // 0x668
  UINT64 DbgCtl;                    // 0x670
  UINT64 BrFrom;                    // 0x678
  UINT64 BrTo;                      // 0x680
  UINT64 LastExcepFrom;             // 0x688
  UINT64 LastExcepTo;               // 0x690
  UINT8 Reserved6[ 0x2E0 - 0x298 ]; // 0x698 (72 bytes)
  UINT64 SpecCtrl;                  // 0x6E0 - Guest SPEC_CTRL
} VMCB_STATE_SAVE, *PVMCB_STATE_SAVE;

// Full VMCB (4KB page)
typedef struct _VMCB {
  VMCB_CONTROL Control;      // 0x000
  VMCB_STATE_SAVE StateSave; // 0x400
} VMCB, *PVMCB;

static_assert( sizeof( VMCB_CONTROL ) == 0x400, "VMCB_CONTROL size mismatch" );

// Exit codes (from AMD APM Vol 2, Appendix C)
#define VMEXIT_CR0_READ            0x0000
#define VMEXIT_CR3_READ            0x0003
#define VMEXIT_CR4_READ            0x0004
#define VMEXIT_CR8_READ            0x0008
#define VMEXIT_CR0_WRITE           0x0010
#define VMEXIT_CR3_WRITE           0x0013
#define VMEXIT_CR4_WRITE           0x0014
#define VMEXIT_CR8_WRITE           0x0018
#define VMEXIT_DR0_READ            0x0020
#define VMEXIT_DR7_READ            0x0027
#define VMEXIT_DR0_WRITE           0x0030
#define VMEXIT_DR7_WRITE           0x0037
#define VMEXIT_EXCEPTION_DE        0x0040
#define VMEXIT_EXCEPTION_DB        0x0041
#define VMEXIT_EXCEPTION_BP        0x0043
#define VMEXIT_EXCEPTION_OF        0x0044
#define VMEXIT_EXCEPTION_BR        0x0045
#define VMEXIT_EXCEPTION_UD        0x0046
#define VMEXIT_EXCEPTION_NM        0x0047
#define VMEXIT_EXCEPTION_DF        0x0048
#define VMEXIT_EXCEPTION_TS        0x004A
#define VMEXIT_EXCEPTION_NP        0x004B
#define VMEXIT_EXCEPTION_SS        0x004C
#define VMEXIT_EXCEPTION_GP        0x004D
#define VMEXIT_EXCEPTION_PF        0x004E
#define VMEXIT_EXCEPTION_MF        0x0050
#define VMEXIT_EXCEPTION_AC        0x0051
#define VMEXIT_EXCEPTION_MC        0x0052
#define VMEXIT_EXCEPTION_XF        0x0053
#define VMEXIT_INTR                0x0060
#define VMEXIT_NMI                 0x0061
#define VMEXIT_SMI                 0x0062
#define VMEXIT_INIT                0x0063
#define VMEXIT_VINTR               0x0064
#define VMEXIT_CR0_SEL_WRITE       0x0065
#define VMEXIT_IDTR_READ           0x0066
#define VMEXIT_GDTR_READ           0x0067
#define VMEXIT_LDTR_READ           0x0068
#define VMEXIT_TR_READ             0x0069
#define VMEXIT_IDTR_WRITE          0x006A
#define VMEXIT_GDTR_WRITE          0x006B
#define VMEXIT_LDTR_WRITE          0x006C
#define VMEXIT_TR_WRITE            0x006D
#define VMEXIT_RDTSC               0x006E
#define VMEXIT_RDPMC               0x006F
#define VMEXIT_PUSHF               0x0070
#define VMEXIT_POPF                0x0071
#define VMEXIT_CPUID               0x0072
#define VMEXIT_RSM                 0x0073
#define VMEXIT_IRET                0x0074
#define VMEXIT_SWINT               0x0075
#define VMEXIT_INVD                0x0076
#define VMEXIT_PAUSE               0x0077
#define VMEXIT_HLT                 0x0078
#define VMEXIT_INVLPG              0x0079
#define VMEXIT_INVLPGA             0x007A
#define VMEXIT_IOIO                0x007B
#define VMEXIT_MSR                 0x007C
#define VMEXIT_TASK_SWITCH         0x007D
#define VMEXIT_FERR_FREEZE         0x007E
#define VMEXIT_SHUTDOWN            0x007F
#define VMEXIT_VMRUN               0x0080
#define VMEXIT_VMMCALL             0x0081
#define VMEXIT_VMLOAD              0x0082
#define VMEXIT_VMSAVE              0x0083
#define VMEXIT_STGI                0x0084
#define VMEXIT_CLGI                0x0085
#define VMEXIT_SKINIT              0x0086
#define VMEXIT_RDTSCP              0x0087
#define VMEXIT_ICEBP               0x0088
#define VMEXIT_WBINVD              0x0089
#define VMEXIT_MONITOR             0x008A
#define VMEXIT_MWAIT               0x008B
#define VMEXIT_MWAIT_COND          0x008C
#define VMEXIT_XSETBV              0x008D
#define VMEXIT_RDPRU               0x008E
#define VMEXIT_EFER_WRITE          0x008F
#define VMEXIT_NPF                 0x0400
#define VMEXIT_AVIC_INCOMPLETE_IPI 0x0401
#define VMEXIT_AVIC_NOACCEL        0x0402
#define VMEXIT_VMGEXIT             0x0403
#define VMEXIT_INVALID             ( -1 )

// Segment attribute bits
#define SVM_SELECTOR_TYPE_MASK ( 0x0F )
#define SVM_SELECTOR_S_SHIFT   4
#define SVM_SELECTOR_S_MASK    ( 1 << SVM_SELECTOR_S_SHIFT )
#define SVM_SELECTOR_DPL_SHIFT 5
#define SVM_SELECTOR_DPL_MASK  ( 3 << SVM_SELECTOR_DPL_SHIFT )
#define SVM_SELECTOR_P_SHIFT   7
#define SVM_SELECTOR_P_MASK    ( 1 << SVM_SELECTOR_P_SHIFT )
#define SVM_SELECTOR_AVL_SHIFT 8
#define SVM_SELECTOR_AVL_MASK  ( 1 << SVM_SELECTOR_AVL_SHIFT )
#define SVM_SELECTOR_L_SHIFT   9
#define SVM_SELECTOR_L_MASK    ( 1 << SVM_SELECTOR_L_SHIFT )
#define SVM_SELECTOR_DB_SHIFT  10
#define SVM_SELECTOR_DB_MASK   ( 1 << SVM_SELECTOR_DB_SHIFT )
#define SVM_SELECTOR_G_SHIFT   11
#define SVM_SELECTOR_G_MASK    ( 1 << SVM_SELECTOR_G_SHIFT )

#pragma pack( pop )
