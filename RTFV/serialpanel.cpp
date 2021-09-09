#include <serialpanel.hpp>
#include <main.hpp>
#include <bitset>

SerialPanel::SerialPanel(wxPanel * parent)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(240,250), wxBORDER_SUNKEN)
{
  _serial = new commSerial();
  wxArrayString baudoptions = getBaudOptions();

  wxBoxSizer *vfullSizer = new wxBoxSizer(wxVERTICAL);
  wxStaticText *panelname = new wxStaticText(this,-1, wxT("Serial connection"));
  vfullSizer->Add(panelname,0,wxALIGN_CENTER_HORIZONTAL|wxTOP,3);

  wxStaticLine *separ = new wxStaticLine(this, -1, wxDefaultPosition, wxSize(90,1));
  separ->Center(wxWidth);
  vfullSizer->Add(separ,0,wxALIGN_CENTER_HORIZONTAL|wxBOTTOM,3);

  _portName = new wxStaticText(this, -1, wxT("Port: ttyUSB0"));
  vfullSizer->Add(_portName,0,wxLEFT,4);


  wxBoxSizer *baudBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText *baudlabel = new wxStaticText(this,-1,wxT("Baud: "));
  _baudSelectBox = new wxChoice(this, ID_BAUDBOX, wxPoint(5,200), wxDefaultSize, baudoptions);
  _baudSelectBox->SetSelection(baudoptions.GetCount()-1);
  baudBoxSizer->Add(baudlabel,0,wxALIGN_CENTER_VERTICAL|wxLEFT,5);
  baudBoxSizer->Add(_baudSelectBox,0,wxLEFT,5);
  vfullSizer->Add(baudBoxSizer,0,0,0);

  _status = new wxStaticText(this, -1, wxT("Status: N/A"));
  vfullSizer->Add(_status,0,wxLEFT,5);

  wxBoxSizer *connButtonSizer = new wxBoxSizer(wxHORIZONTAL);
  _conn = new wxButton(this, ID_CONN, wxT("Connect"));
  connButtonSizer->Add(_conn,0,0,0);
  _disc = new wxButton(this, ID_DISC, wxT("Disconnect"));
  connButtonSizer->Add(_disc,0,0,0);
  vfullSizer->Add(connButtonSizer,0,wxALIGN_CENTER_HORIZONTAL,0);

  _msg = new wxStaticText(this, -1, wxT("Message: "));
  vfullSizer->Add(_msg,0,wxLEFT,5);

  this->SetSizer(vfullSizer);
  Layout();

  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SerialPanel::OnConnect, this, ID_CONN);
  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SerialPanel::OnDisconnect, this, ID_DISC);
}

SerialPanel::~SerialPanel()
{
  delete _serial;
}

void SerialPanel::OnConnect(wxCommandEvent & WXUNUSED(event))
{
  int st = -128;

  if(_serial==NULL){
    _serial = new commSerial();
    st = _serial->connect(_baudSelectBox->GetString(_baudSelectBox->GetSelection()).ToStdString());
  }else{
    st = _serial->connect(_baudSelectBox->GetString(_baudSelectBox->GetSelection()).ToStdString());
  }

  if(st==1){ // Successful connection
    _conn->Enable(false);
    _baudSelectBox->Enable(false);
  }
  UpdateStatus(_serial->statusconvert(st));
}

void SerialPanel::OnDisconnect(wxCommandEvent & WXUNUSED(event))
{
  int st = _serial->disconnect();
  UpdateStatus(_serial->statusconvert(st));
  _conn->Enable(true);
  _baudSelectBox->Enable(true);
}

void SerialPanel::UpdateStatus(const char* newstatus)
{
  std::string a = "Status: ";
  a+=newstatus;
  _status->SetLabel(a);
}

void SerialPanel::UpdateMessage(wxCommandEvent & event)
{
  _msg->SetLabel("Message: " + event.GetString());
}

wxArrayString SerialPanel::getBaudOptions()
{
  std::vector<std::string> bauds = _serial->availableBauds;

  wxArrayString baudoptions;
  for(const std::string &baud : bauds)
  {
    baudoptions.Add(baud);
  }
  return baudoptions;
}
