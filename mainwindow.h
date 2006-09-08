#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/table.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/label.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/button.h>
#include <glibmm/thread.h>
#include <gtkmm/adjustment.h>

#include "ciso.h"

class MainWindow
	: public Gtk::Window
{
public:
    MainWindow();
    virtual ~MainWindow();

protected:
	void init();
	void initStatusBar();
	void pushStatusMessage(unsigned int context_id, std::string message);
    
	void onStart();
	void onFileSelected();
	void quit();

	void enableControls();
	void disableControls();
	
	void openFile(std::string filename);
	bool update();

	Gtk::Table 				m_Layout;
	
	Gtk::Statusbar 			m_StatusBar;
	Gtk::Label				m_FileOpenLabel;
	Gtk::Label				m_CompressionLabel;
	Gtk::FileChooserButton	m_FileChooserButton;
	Gtk::Adjustment			m_CompressionAdjustment;
	Gtk::SpinButton			m_CompressionInput;
	Gtk::ProgressBar		m_ProgressBar;
	Gtk::Button				m_StartButton;
	guint 					m_ContextId;
    
	CIso					m_IsoCompressor;
	Glib::Thread*			m_CompressorThread;
	sigc::connection 		m_TimerConnection;
};

#endif
