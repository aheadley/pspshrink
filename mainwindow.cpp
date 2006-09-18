#include "mainwindow.h"

#include <iostream>
#include <sstream>
#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/filefilter.h>

using namespace Gtk;
using namespace std;

MainWindow::MainWindow()
: m_Layout(7, 2, false)
, m_FileOpenLabel("Open file:", ALIGN_LEFT)
, m_CompressionLabel("Compression level:", ALIGN_LEFT)
, m_CompressionAdjustment(9.0, 1.0, 9.0)
, m_CompressionInput(m_CompressionAdjustment)
, m_StartButton("Start")
, m_CompressorThread(NULL)
{
	#ifndef NDEBUG
		cout << "DEBUGING" << endl;
	#endif
	
	set_title("PSP ISO Compressor");
	set_border_width(5);
	set_size_request(400, 200);
	//set_resizable(false);
	
	init();
	initStatusBar();
	
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
	m_FileChooserButton.set_size_request(390);
	//m_ProgressBar.set_size_request(-1, 30);
	m_StartButton.set_sensitive(false);
	
	FileFilter fileFilter;
	fileFilter.add_pattern("*.iso");
	fileFilter.add_pattern("*.cso");
	m_FileChooserButton.set_filter(fileFilter);
		
	m_Layout.attach(m_FileOpenLabel, 0, 2, 0, 1, FILL, FILL);
	m_Layout.attach(m_FileChooserButton, 0, 2, 1, 2, FILL, EXPAND);
	m_Layout.attach(m_CompressionLabel, 0, 1, 2, 3, FILL, FILL);
	m_Layout.attach(m_CompressionInput, 1, 2, 2, 3, FILL, EXPAND);
	m_Layout.attach(m_ProgressBar, 0, 2, 3, 4, FILL, EXPAND);
	m_Layout.attach(m_StartButton, 1, 2, 4, 5, FILL, FILL);
}


void MainWindow::initStatusBar()
{
	m_StatusBar.set_has_resize_grip(false);
	m_ContextId = m_StatusBar.get_context_id("Ready");
	m_Layout.attach(m_StatusBar, 0, 2, 5, 6, FILL, SHRINK);
}

void MainWindow::enableControls()
{
	m_FileChooserButton.set_sensitive(true);
	m_StatusBar.set_sensitive(true);
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
	
	ss << progress << " %";
	m_ProgressBar.set_fraction(progress / 100.f);
	m_ProgressBar.set_text(ss.str());
		

	if(progress == 100)
	{
		m_FileChooserButton.set_sensitive(true);
		enableControls();
		pushStatusMessage(m_ContextId, "Operation complete");
		return false;
	}
	
	return true;

	//TODO do something with the compression
}


void MainWindow::openFile(string filename)
{
	
}


void MainWindow::pushStatusMessage(unsigned int context_id, string message)
{
	m_StatusBar.push(message.c_str(), context_id);
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
		cout << filenameOut << endl;
		m_CompressorThread = Glib::Thread::create(sigc::bind(sigc::mem_fun(m_IsoCompressor, &CIso::compress), filenameIn, filenameOut, (int) m_CompressionInput.get_value()), false);
		assert(m_CompressorThread);
	}
	else if(extension == ".cso")
	{
		filenameOut = filenameIn.substr(0, filenameIn.length() - 4) + ".iso";
		cout << filenameOut << endl;
		m_CompressorThread = Glib::Thread::create(sigc::bind(sigc::mem_fun(m_IsoCompressor, &CIso::decompress), filenameIn, filenameOut), false);
		assert(m_CompressorThread);
	}

	m_TimerConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &MainWindow::update), 100);
}

void MainWindow::onFileSelected()
{
	m_StartButton.set_sensitive(true);
}

