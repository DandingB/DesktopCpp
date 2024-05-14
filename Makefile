ifeq ($(OS),Windows_NT)

CXX = g++ -std=gnu++17
LIBS = -lopengl32
FLAGS = -mwindows -municode --static


default: compile

compile:
	$(CXX) src/main.cpp src/platform/Windows.cpp $(LIBS) $(FLAGS) -o bin/App.exe

else

CXX = g++ -std=gnu++17 -mmacosx-version-min=10.10
FRAMEWORKS = -framework Cocoa -framework AppKit -framework OpenGL -framework System


default: compile

compile:
	$(CXX) src/main.cpp src/platform/MacOS.mm $(FRAMEWORKS) -o bin/App

endif

