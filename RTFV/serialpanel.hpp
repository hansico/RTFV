#ifndef SERIALPANEL_HPP
#define SERIALPANEL_HPP

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <commserial.hpp>

class SerialPanel : public wxPanel
{
public:
  SerialPanel(wxPanel *parent);
  ~SerialPanel();
  
  void OnConnect(wxCommandEvent & event);
  void OnDisconnect(wxCommandEvent & event);
  void UpdateMessage(wxCommandEvent & event);
  
private:
  wxStaticText *_status;
  wxStaticText *_portName;
  wxStaticText *_msg;
  wxButton *_disc;
  wxButton * _conn;
  wxChoice *_baudSelectBox;
  commSerial *_serial;

  void UpdateStatus(const char *newstatus);
  wxArrayString getBaudOptions();
};

const int ID_CONN = 101;
const int ID_DISC = 102;
const int ID_BAUDBOX = 103;
#endif // SERIALPANEL_HPP
