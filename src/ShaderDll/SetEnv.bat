::call F:\VisualStudio2019\Professional\Common7\Tools\VsDevCmd.bat
call F:\VisualStudio2019\Professional\VC\Auxiliary\Build\vcvars64.bat

::this is the equaivalent implement of cl /LD
::cl -c ShaderDll.cpp
::link -dll -out:ShaderDll.dll ShaderDll.obj
cl /LD ShaderDll.cpp 

pause