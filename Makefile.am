bin_PROGRAMS            =   pspshrink pspshrinkui

pspshrink_SOURCES       =   src/pspshrink.cpp \
                            src/ciso.h src/ciso.cpp

pspshrink_LDADD         =   -lz -lpthread

pspshrinkui_SOURCES     =   src/pspshrinkgui.cpp \
                            src/ciso.h src/ciso.cpp \
                            src/mainwindow.h src/mainwindow.cpp
                        

pspshrinkui_CXXFLAGS    =   $(GTKMM_CFLAGS) -DDATA_DIR=\""$(datadir)"\"
pspshrinkui_LDADD       =   $(GTKMM_LIBS) $(GTHREAD_LIBS) -lz

desktopdir              =   $(datadir)/applications
desktop_DATA            =   data/pspshrink.desktop

pixmapsdir              =   $(datadir)/pixmaps
pixmaps_DATA            =   data/pspshrink.svg

EXTRA_DIST              =   $(desktop_DATA) $(pixmaps_DATA)
