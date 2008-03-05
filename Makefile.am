bin_PROGRAMS            =   pspshrink pspshrinkgui

pspshrink_SOURCES       =   src/pspshrink.cpp \
                            src/ciso.h src/ciso.cpp

pspshrink_LDADD         =   -lz -lpthread

pspshrinkgui_SOURCES    =   src/pspshrinkgui.cpp \
                            src/ciso.h src/ciso.cpp \
                            src/mainwindow.h src/mainwindow.cpp
                        

pspshrinkgui_CXXFLAGS   =   $(GTKMM_CFLAGS)
pspshrinkgui_LDADD      =   $(GTKMM_LIBS) $(GTHREAD_LIBS) -lz

desktopdir              =   $(datadir)/applications
desktop_DATA            =   data/pspshrink.desktop

EXTRA_DIST              =   $(desktop_DATA)