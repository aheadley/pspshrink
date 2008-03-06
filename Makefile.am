bin_PROGRAMS            =   pspshrink pspshrinkui

pspshrink_SOURCES       =   src/pspshrink.cpp \
                            src/ciso.h src/ciso.cpp

pspshrink_LDADD         =   -lz -lpthread

pspshrinkui_SOURCES     =   src/pspshrinkgui.cpp \
                            src/ciso.h src/ciso.cpp \
                            src/mainwindow.h src/mainwindow.cpp
                        

pspshrinkui_CXXFLAGS    =   $(GTKMM_CFLAGS)
pspshrinkui_LDADD       =   $(GTKMM_LIBS) $(GTHREAD_LIBS) -lz

desktopdir              =   $(datadir)/applications
desktop_DATA            =   data/pspshrink.desktop

EXTRA_DIST              =   $(desktop_DATA)