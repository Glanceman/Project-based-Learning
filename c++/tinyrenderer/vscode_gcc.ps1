# Set environment variables
$env:VSCODE = "D:\VSCode\bin"
#$env:GCC = "D:\mingw64\bin"

# Modify PATH environment variable
$env:PATH = "$env:PATH;$env:VSCODE"

# Display the updated PATH
Write-Host $env:PATH

code .