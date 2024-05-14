ifeq ($(OS),Windows_NT)

CXX = g++ -std=gnu++17
LIBS = -lopengl32 -mwindows


default: compile

compile:
	$(CXX) src/main.cpp src/platform/Windows.cpp $(LIBS) -municode --static -o bin/App.exe

else

CXX = g++ -std=gnu++17 -mmacosx-version-min=10.10
FRAMEWORKS = -framework Cocoa -framework AppKit -framework OpenGL -framework System


default: compile

compile:
	$(CXX) src/main.cpp src/platform/MacOS.mm $(FRAMEWORKS) -o bin/App

endif

