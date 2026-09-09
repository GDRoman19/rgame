pong: traylib.o
	g++ pangpong.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -c -o pangpong.o
	g++ pangpong.o traylib.o -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -o pangpong.exe
traylib.o: traylib.cpp
	g++ traylib.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -c -o traylib.o
button:
	g++ traylib.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -c -o traylib.o
	g++ button.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -c -o button.o
	g++ button.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm traylib.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -o button.exe
line:
	g++ lines.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -o lines.exe
mrstup: traylib.o
	g++ mrstoop.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -c -o mrstoop.o
	g++ mrstoop.o traylib.o -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -o mrstoop.exe
block: objgen.cpp
	g++ objgen.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -o block.exe
screen: screen.cpp
	g++ screen.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -o screen.exe
vvvvvv: vvvvvvE.cpp
	g++ vvvvvvE.cpp -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -o vvvvvvE.exe
