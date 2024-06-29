@echo off

REM compile shaders

set vertexName=vertex
set fragmentName=fragment

set shaderDir=..\..\build\shader
set toolDir=..\..\build\vcpkg_installed\x64-windows\tools\bgfx
set includePath=..\..\build\vcpkg_installed\x64-windows\include\bgfx

if not exist %shaderDir% mkdir %shaderDir%

REM simple shader
%toolDir%\shaderc.exe ^
-f %vertexName%.sc -o %shaderDir%\%vertexName%.bin ^
--platform windows --type vertex --verbose -i %includePath%/ -p s_5_0

%toolDir%\shaderc.exe ^
-f %fragmentName%.sc -o %shaderDir%\%fragmentName%.bin ^
--platform windows --type fragment --verbose -i %includePath%/ -p s_5_0
