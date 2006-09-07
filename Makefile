CC = g++
LD = g++ -D NDEBUG
CFLAGS = -c -Wall

LIBS=-lz -lpthread
GUILIBS=`pkg-config gtkmm-2.4 --cflags --libs` `pkg-config --libs gthread-2.0`
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
