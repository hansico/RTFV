#ifndef COMMSERIAL_HPP
#define COMMSERIAL_HPP

#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <iostream>
#include <thread>
#include <future>
#include <wx/wx.h>

class SerialPanel;

class commSerial
{
public:
  commSerial(SerialPanel *parent);
  ~commSerial();
  int connect(std::string baudstr);
  int disconnect();
  std::vector<std::string> availableBauds = {"9600", "38400", "57600", "115200"};
  const char* statusconvert(int stcode);

private:
  SerialPanel *_parent;
  bool open_port();
  bool config(std::string baudstr);
  void readserial(std::future<void> future);
  bool connected = false;

  struct termios _settings;
  bool setBaud(std::string baudstr);
  enum _statusCodes {ST_UNRECOG_BAUD = -2, ST_NOT_AVLBL, ST_DISCONN, ST_CONN=1};
  int _dev;
  uint8_t _buff[32];

  std::thread * _workerThr;
  std::promise<void> _exit_signal;
};

wxDEFINE_EVENT(UPD_MSG,wxCommandEvent);

#endif // COMMSERIAL_HPP
