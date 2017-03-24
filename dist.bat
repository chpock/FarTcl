@call "%VS100COMNTOOLS%vsvars32.bat"
@for /f %%i in ('tcl8.6\tclsh86t.exe tools\getversion.tcl 2^>nul') do set VERS=%%i
@nmake -f makefile.vc DEBUG=1 clean
@nmake -f makefile.vc DEBUG=1 srcdist
@nmake -f makefile.vc DEBUG=1 dist PREFIX="dist\FarTcl"
@rd /q /s "dist" 2>nul >nul

