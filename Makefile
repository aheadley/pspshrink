CC = g++
LD = g++
CFLAGS = -c -Wall

LIBS=-lz
GUILIBS=`pkg-config gtkmm-2.4 --cflags --libs`
GTKMMINCLUDES=`pkg-config gtkmm-2.4 --cflags`


pspshrinkgui: pspshrinkgui.cpp mainwindow.o ciso.o
	$(LD) $^ $(LIBS) $(GUILIBS) -o $@
	
pspshrink: pspshrink.cpp ciso.o
	$(LD) $^ $(LIBS) -o $@

mainwindow.o: mainwindow.h mainwindow.cpp
	$(CC) $(CFLAGS) $(GTKMMINCLUDES) mainwindow.cpp
    
ciso.o: ciso.h ciso.cpp
	$(CC) $(CFLAGS) ciso.cpp
	
clean:
	rm *.o
