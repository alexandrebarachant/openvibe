call win32-init_env_command.cmd

mkdir ..\local-tmp
cd ..\local-tmp
nmake clean
cd ..

rmdir /s /q local-tmp
