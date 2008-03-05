CC = g++
LD = g++
UIC = uic
MOC = moc
CFLAGS = -c -D NDEBUG -Wall

LIBS=-lz -lpthread
GTKGUILIBS=`pkg-config gtkmm-2.4 --cflags --libs` `pkg-config --libs gthread-2.0`
GTKMMINCLUDES=`pkg-config gtkmm-2.4 --cflags`
QTINCLUDES=-I$(QTDIR)/include
QTGUILIBS=-L$(QTDIR)/lib -lqt
QTUI=pspshrink.ui
QTUIHDR=pspshrinkqt.h
QTUISRC=pspshrinkqt.cpp



gtkgui: pspshrinkgui.cpp mainwindow.o ciso.o
	$(LD) $^ $(LIBS) $(GTKGUILIBS) -o $@

qtgui: $(QTUI)
	$(UIC) $(QTUI) > $(QTUIHDR)
	$(UIC) -impl $(QTUIHDR) $(QTUI) > $(QTUISRC)
	$(MOC) -o moc_$(QTUISRC) $(QTUIHDR)
#	$(MOC) -o moc_$(QTUISRC) $(QTUIHDR)

pspshrink: pspshrink.cpp ciso.o
	$(LD) $^ $(LIBS) -o $@

mainwindow.o: mainwindow.h mainwindow.cpp
	$(CC) $(CFLAGS) $(GTKMMINCLUDES) mainwindow.cpp
    
ciso.o: ciso.h ciso.cpp
	$(CC) $(CFLAGS) ciso.cpp
	
clean:
	rm *.o
