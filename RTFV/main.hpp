#ifndef  MAIN_HPP
#define MAIN_HPP

#include <wx/wx.h>
#include <messagehandler.hpp>
#include <mainwindow.hpp>

class wxMiniApp : public wxApp
{
public:
  virtual bool OnInit();
  
  MainWindow *_mainwindow;
  MessageHandler *_msgHandler;
};

DECLARE_APP( wxMiniApp );
#endif // MAIN_HPP
