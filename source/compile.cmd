@echo off
if exist pov_launch.exe del pov_launch.exe > nul
icc /Gd+ /Gm+ /B" /pmtype:pm" pov_launch.cpp p_crop.cpp p_file.cpp p_finish.cpp p_output.cpp p_render.cpp p_size.cpp view.cpp
if exist pov_launch.exe rc -x2 -p pov_launch.rc pov_launch.exe
if exist pov_launch.exe start /n pov_launch.exe

