@call "%VS100COMNTOOLS%vsvars32.bat"
@nmake -f makefile.vc INSTALL DEBUG=1 PREFIX="C:\Program Files\Far Manager\Plugins\FarTcl"