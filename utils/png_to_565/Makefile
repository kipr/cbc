INCPATH = -I/usr/local/Qt4.4/mkspecs/macx-g++ -I. -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/usr/include/QtCore -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/usr/include/QtGui

LIBS = -F/Library/Frameworks -L/Library/Frameworks -framework QtGui -framework Carbon -framework AppKit -framework QtCore -lz -lm -framework ApplicationServices

png_to_565: png_to_565.cpp
	g++ -Wall $(INCPATH) png_to_565.cpp -o png_to_565 $(LIBS)
