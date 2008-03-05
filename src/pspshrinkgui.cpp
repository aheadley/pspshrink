#include <gtkmm/main.h>
#include <glibmm/thread.h>

#include "mainwindow.h"

int main(int argc, char **argv)
{ 
	Glib::thread_init();	
    Gtk::Main m(&argc, &argv);

	MainWindow mainWindow;
	Gtk::Main::run(mainWindow);
    
    return 0;
}
