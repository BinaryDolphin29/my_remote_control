# my_remote_control
  
As remote control of CL8D-5.0(アイリスオーヤマ) is troublesome to push the button twice, I made this.  
Raspberry Pi Pico is used.  
```cmd
rem mkdir ..\build && cd ..\build
cmake -G "MinGW Makefiles" ..
make -j 12
cp .\my_remote_control\my_remote_control.uf2 F:\
```