ifeq ($(OS),Windows_NT)

###########
# Windows #
###########

CXX = g++ -std=gnu++17
LIBS = -lopengl32
FLAGS = -mwindows -municode --static


default: compile

compile:
	$(CXX) src/main.cpp src/cx/WindowContainer.cpp src/platform/ImplWindows.cpp $(LIBS) $(FLAGS) -o bin/App.exe

else

#########
# MacOS #
#########

CXX = g++ -std=gnu++17 -mmacosx-version-min=10.10
FRAMEWORKS = -framework Cocoa -framework AppKit -framework OpenGL -framework System


default: compile

compile:
	$(CXX) src/main.cpp src/cx/WindowContainer.cpp src/platform/ImplMacOS.mm $(FRAMEWORKS) -o bin/App

bundle:
	mkdir -p bin/App.app/Contents
	mkdir -p bin/App.app/Contents/MacOS
	mkdir -p bin/App.app/Contents/Resources
	
	sed -e "s/\$${MACOSX_BUNDLE_GUI_IDENTIFIER}/org.crispycross.App/" \
	-e "s/\$${MACOSX_BUNDLE_EXECUTABLE_NAME}/App/" \
	-e "s/\$${MACOSX_BUNDLE_BUNDLE_NAME}/App/" \
	-e "s/\$${MACOSX_BUNDLE_COPYRIGHT}/Copyright 2024 CrispyCross/" \
	-e "s/\$${MACOSX_BUNDLE_BUNDLE_VERSION}/$(WX_VERSION)/" \
	-e "s/\$${MACOSX_BUNDLE_INFO_STRING}/wxTest version $(WX_VERSION), (c) 2002-2023 CrispyCross/" \
	-e "s/\$${MACOSX_BUNDLE_LONG_VERSION_STRING}/$(WX_VERSION), (c) 2002-2023 CrispyCross/" \
	-e "s/\$${MACOSX_BUNDLE_SHORT_VERSION_STRING}/$(WX_RELEASE)/" \
	support_files/Info.plist.in > bin/App.app/Contents/Info.plist
	
	/bin/echo "APPL????" >bin/App.app/Contents/PkgInfo
	
	ln -f bin/App bin/App.app/Contents/MacOS/App
	
	# cp -f support_files/media/icon.icns bin/App.app/Contents/Resources/icon.icns


endif

