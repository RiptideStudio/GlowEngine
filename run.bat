@echo on

:: Copy the assets into the x64
xcopy /y /e "GlowEngine\Assets" "x64\Debug\Assets\"

:: Navigate to the debug folder
cd /d "%~dp0x64\Debug"
