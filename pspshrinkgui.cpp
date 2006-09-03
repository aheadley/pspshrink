#include <gtkmm/main.h>

#include "mainwindow.h"

int main(int argc, char **argv)
{  
    Gtk::Main m(&argc, &argv);

    MainWindow mainWindow;
    m.run(mainWindow);
    
    return 0;
}
