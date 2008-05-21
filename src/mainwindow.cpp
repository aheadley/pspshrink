#include "mainwindow.h"

#include <assert.h>
#include <iostream>
#include <sstream>
#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/filefilter.h>

using namespace Gtk;
using namespace std;

MainWindow::MainWindow()
: m_Layout(6, 2, false)
, m_CompressionLabel("Compression level:", ALIGN_LEFT)
, m_CompressionAdjustment(9.0, 1.0, 9.0)
, m_CompressionInput(m_CompressionAdjustment)
, m_StartButton("Start")
, m_CompressorThread(NULL)
{
    #ifndef NDEBUG
        cout << "DEBUGGING" << endl;
    #endif
    
    set_title("PspShrink");
    set_border_width(7);
    set_size_request(350, 200);
    set_resizable(false);
    if (!set_icon_from_file(string(DATA_DIR) + "/pixmaps/pspshrink.svg"))
    {
        cerr << "Failed to set application icon from: " << string(DATA_DIR) + "/pixmaps/pspshrink.svg" << endl;
    }
    cout << string(DATA_DIR) + "/pixmaps/pspshrink.svg" << endl;
    
    init();
    
    m_StartButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onStart));
    m_FileChooserButton.signal_selection_changed().connect(sigc::mem_fun(*this, &MainWindow::onFileSelected));

    add(m_Layout);
    show_all_children();
}

MainWindow::~MainWindow()
{
}

void MainWindow::quit()
{
    Main::quit();
}

void MainWindow::init()
{
    m_CompressionInput.set_digits(0);
    m_CompressionInput.set_value(9);
    m_FileChooserButton.set_size_request(336);
    m_ProgressBar.set_size_request(-1, 30);
    m_CompressionRateBar.set_size_request(-1, 20);
    m_StartButton.set_sensitive(false);
    
    FileFilter fileFilter;
    fileFilter.add_pattern("*.iso");
    fileFilter.add_pattern("*.cso");
    m_FileChooserButton.set_filter(fileFilter);
        
    m_Layout.attach(m_FileChooserButton, 0, 2, 0, 1, FILL, EXPAND);
    m_Layout.attach(m_CompressionLabel, 0, 1, 1, 2, FILL, FILL);
    m_Layout.attach(m_CompressionInput, 1, 2, 1, 2, FILL, EXPAND);
    m_Layout.attach(m_ProgressBar, 0, 2, 2, 3, FILL, EXPAND);
    m_Layout.attach(m_CompressionRateBar, 0, 2, 3, 4, FILL, EXPAND);
    m_Layout.attach(m_StartButton, 1, 2, 4, 5, FILL, FILL);
}


void MainWindow::enableControls()
{
    m_FileChooserButton.set_sensitive(true);
    m_StartButton.set_sensitive(true);
    m_CompressionInput.set_sensitive(true);
}

void MainWindow::disableControls()
{
    m_FileChooserButton.set_sensitive(false);
    m_StartButton.set_sensitive(false);
    m_CompressionInput.set_sensitive(false);
}

bool MainWindow::update()
{
    stringstream ss;
    int progress = m_IsoCompressor.getProgress();
    int compression = m_IsoCompressor.getCompressionRate();
    
    ss << progress << " %";
    m_ProgressBar.set_fraction(progress / 100.f);
    m_ProgressBar.set_text(ss.str());
    
    if(compression != 0)
    {
        ss.str("");
        ss << "compression: " << compression << " %";
        m_CompressionRateBar.set_fraction(compression / 100.f);
        m_CompressionRateBar.set_text(ss.str());
    }
        
    
    if(progress == 100)
    {
        enableControls();
        return false;
    }
    
    return true;
}


void MainWindow::onStart()
{
    string filenameIn = m_FileChooserButton.get_filename();
    string extension = filenameIn.substr(filenameIn.length() - 4, filenameIn.length());
    string filenameOut;

    disableControls();
    
    if(extension == ".iso")
    {
        filenameOut = filenameIn.substr(0, filenameIn.length() - 4) + ".cso";
        m_CompressorThread = Glib::Thread::create(sigc::bind(sigc::mem_fun(m_IsoCompressor, &CIso::compress), filenameIn, filenameOut, (int) m_CompressionInput.get_value()), false);
        assert(m_CompressorThread);
    }
    else if(extension == ".cso")
    {
        filenameOut = filenameIn.substr(0, filenameIn.length() - 4) + ".iso";
        m_CompressorThread = Glib::Thread::create(sigc::bind(sigc::mem_fun(m_IsoCompressor, &CIso::decompress), filenameIn, filenameOut), false);
        assert(m_CompressorThread);
    }

    m_TimerConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &MainWindow::update), 100);
}

void MainWindow::onFileSelected()
{
    m_StartButton.set_sensitive(true);
    
    m_CompressionRateBar.set_fraction(0);
    m_CompressionRateBar.set_text("");

}

