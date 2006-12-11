call win32-init_env_command.cmd

cd ..
md local-tmp
cd local-tmp
cmake .. -G"NMake Makefiles"
nmake

pause