# Morpheus

An AMD-V bluepill hypervisor for Windows. Virtualizes the running OS from underneath, trapping it in a matrix it can't detect or escape.

Named after Morpheus from The Matrix, the one who offers the choice between red pill and blue pill. Except here, there is no choice.

## Setup

1. Install [VS2022](https://aka.ms/vs/17/release/vs_community.exe) with "Desktop development with C++" and "Windows Driver Kit"
2. Install [WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)

## Running

### VMware (Recommended)

Hyper-V nested AMD-V is buggy. VMware (Workstation 25h2 in my testing) is more reliable.

**On HOST - disable Hyper-V:**
```cmd
reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v LsaCfgFlags /t REG_DWORD /d 0 /f
reg add "HKLM\SYSTEM\CurrentControlSet\Control\DeviceGuard" /v EnableVirtualizationBasedSecurity /t REG_DWORD /d 0 /f
bcdedit /set hypervisorlaunchtype off
```

Also disable Memory Integrity: Windows Security → Device Security → Core Isolation → OFF

Reboot and verify with `msinfo32` - "Virtualization-based security" should say "Not enabled"

**In VMware:**
1. Create Windows 10/11 VM
2. VM Settings → Processors → Enable "Virtualize AMD-V/RVI"
3. In guest: `bcdedit /set hypervisorlaunchtype off` + disable Memory Integrity
4. Load driver

### Hyper-V

Run in admin PowerShell on HOST:
```powershell
Stop-VM -Name "Test" -TurnOff
Set-VMComPort -VMName "Test" -Number 1 -Path \\.\pipe\morpheus
Set-VMProcessor -VMName "Test" -ExposeVirtualizationExtensions $true
Start-VM -Name "Test"
```

In guest VM: `bcdedit /set hypervisorlaunchtype off`

## COM Port Debug Output

View output with: `powershell -ExecutionPolicy Bypass -File comport.ps1`