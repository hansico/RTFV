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
  void connect();

private:
  SerialPanel *_parent;
  bool open_port();
  void disconnect();
  void config();
  void readserial(std::future<void> future);

  struct termios _settings;
  int _dev;
  uint8_t _buff[32];
  
  std::thread * _workerThr;
  std::promise<void> _exit_signal;
};

wxDEFINE_EVENT(UPD_STATUS,wxCommandEvent);
wxDEFINE_EVENT(UPD_MSG,wxCommandEvent);

#endif // COMMSERIAL_HPP
