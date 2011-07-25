@echo off
reimport.exe class.exe output.exe hook.dll winmm.dll::waveOutWrite
rem ***reimport.exe class.exe output.exe hook.dll kernel32.dll::LoadLibraryA kernel32.dll::GetCommandLineA user32.dll::MessageBoxA
rem ***reimport.exe fifa2004.exe output.exe hook.dll kernel32.dll::GetModuleHandleA
reimport.exe
pause