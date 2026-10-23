@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64

cl.exe /std:c++17 /EHsc /Zi ^
/I "C:\Users\adars\Downloads\mysql-connector-c++-8.0.33-winx64\mysql-connector-c++-8.0.33-winx64\include" ^
main.cpp ^
/link ^
/LIBPATH:"C:\Users\adars\Downloads\mysql-connector-c++-8.0.33-winx64\mysql-connector-c++-8.0.33-winx64\lib64\vs14" ^
mysqlcppconn.lib ^
/OUT:airline.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo BUILD SUCCESSFUL
    echo ========================================
) else (
    echo.
    echo ========================================
    echo BUILD FAILED
    echo ========================================
)

pause