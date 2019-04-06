@echo off

set "console-gui=console-gui\"
set "includes= "
set "arg=-DGCC -o a.exe -ggdb -w"

::console-gui Submodule

set "console-gui-sourcePath=%console-gui%source\"
set "console-gui-headerPath=%console-gui%headers\"
set "sourcePath=%console-gui-sourcePath%"
set "headerPath=%console-gui-headerPath%"

set "includes=%includes% -I%headerPath%"
set "sources=%sourcePath%gameEngine.cpp %sourcePath%imath.cpp %sourcePath%lodepng.cpp %sourcePath%pixel.cpp %sourcePath%png.cpp %sourcePath%rigidbody.cpp %sourcePath%rigidbox.cpp"

set "sources=%sources% agarioGame.cpp agarioGameMap.cpp gamecamera.cpp playerOrb.cpp"

@echo on
%userprofile%\Downloads\pocketcpp-0.7\pocketcpp\MinGW\bin\g++ %includes% %sources% %arg% console_agario_cpp.cpp
if "%errorlevel%" NEQ "0" (
pause
)
@echo off
