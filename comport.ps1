# serial_monitor.ps1
$pipeName = "morpheus"

Write-Host "Connecting to \\.\pipe\$pipeName ..." -ForegroundColor Cyan

try {
    $pipe = New-Object System.IO.Pipes.NamedPipeClientStream(".", $pipeName, [System.IO.Pipes.PipeDirection]::In)
    $pipe.Connect(5000)  # 5 sec timeout instead of hanging forever
    Write-Host "Connected. Waiting for output... (Ctrl+C to exit)`n" -ForegroundColor Green
    
    $reader = New-Object System.IO.StreamReader($pipe)
    while ($pipe.IsConnected) {
        $line = $reader.ReadLine()
        if ($line) { Write-Host $line }
    }
}
catch [TimeoutException] {
    Write-Host "Connection timed out. Is the VM running?" -ForegroundColor Red
}
catch {
    Write-Host "Error: $_" -ForegroundColor Red
}
finally {
    if ($reader) { $reader.Dispose() }
    if ($pipe) { $pipe.Dispose() }
}