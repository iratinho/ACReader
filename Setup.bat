powershell -command "new-item -type directory -path .tmp -Force"
powershell -command "Start-BitsTransfer -Source https://github.com/premake/premake-core/releases/download/v5.0.0-beta1/premake-5.0.0-beta1-windows.zip -Destination .tmp/premake.zip"
powershell -command "Expand-Archive .tmp/premake.zip . -Force"
powershell -command "Remove-Item '.tmp' -Recurse"
