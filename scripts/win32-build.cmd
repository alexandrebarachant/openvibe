call win32-init_env_command.cmd

cd ..
cd local-tmp
cmake .. -G"NMake Makefiles"
nmake

pause