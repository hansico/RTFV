#include <main.hpp>

IMPLEMENT_APP(wxMiniApp);

bool wxMiniApp::OnInit()
{
  MainWindow *mainwindow = new MainWindow(wxT("RTFV"));
  _mainwindow = mainwindow;
  mainwindow->Show();
  
  _msgHandler = new MessageHandler();
  return true;
}
