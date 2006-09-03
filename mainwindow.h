#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/table.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/label.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/scale.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/button.h>
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
	
    void openFile(std::string filename);

    Gtk::Table 				m_Layout;
    //Glib::RefPtr<Gtk::UIManager> m_UIManager;
	
    Gtk::Statusbar 			m_StatusBar;
    Gtk::Label				m_FileOpenLabel;
    Gtk::Label				m_CompressionLabel;
    Gtk::FileChooserButton	m_FileChooserButton;
    Gtk::HScale				m_CompressionSlider;
    Gtk::ProgressBar		m_ProgressBar;
    Gtk::Button				m_StartButton;
    guint 					m_ContextId;
    
    CIso					m_IsoCompressor;
};

#endif
