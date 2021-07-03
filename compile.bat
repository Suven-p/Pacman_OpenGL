g++.exe -Wall -std=c++17 -g -Iinclude -Ifreeglut\include -I".\freeglut\include" -c ".\src\game.cpp" -o obj\Windows\src\game.o
gcc.exe -Wall -std=c17 -g -std=c17 -Iinclude -Ifreeglut\include -I".\freeglut\include" -c ".\src\glad.c" -o obj\Windows\src\glad.o
g++.exe -Wall -std=c++17 -g -Iinclude -Ifreeglut\include -I".\freeglut\include" -c ".\src\main.cpp" -o obj\Windows\src\main.o
g++.exe -Wall -std=c++17 -g -Iinclude -Ifreeglut\include -I".\freeglut\include" -c ".\src\map.cpp" -o obj\Windows\src\map.o
g++.exe -Wall -std=c++17 -g -Iinclude -Ifreeglut\include -I".\freeglut\include" -c ".\src\resourceManager.cpp" -o obj\Windows\src\resourceManager.o
g++.exe -Wall -std=c++17 -g -Iinclude -Ifreeglut\include -I".\freeglut\include" -c ".\src\shader.cpp" -o obj\Windows\src\shader.o
g++.exe -Wall -std=c++17 -g -Iinclude -Ifreeglut\include -I".\freeglut\include" -c ".\src\texture.cpp" -o obj\Windows\src\texture.o

g++.exe -Lfreeglut\lib\x64 -L".\freeglut\lib\x64" -o bin\project_1.exe obj\Windows\src\game.o obj\Windows\src\glad.o obj\Windows\src\main.o obj\Windows\src\map.o obj\Windows\src\resourceManager.o obj\Windows\src\shader.o obj\Windows\src\texture.o  -lfreeglut -lopengl32 -lglu32
pause
