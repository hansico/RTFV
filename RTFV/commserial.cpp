#include <commserial.hpp>
#include <serialpanel.hpp>
#include <main.hpp>

commSerial::commSerial(SerialPanel * parent)
{
  _parent = parent;
  _workerThr = NULL;

  _parent->Bind(UPD_STATUS, &SerialPanel::UpdateStatus,_parent);
  _parent->Bind(UPD_MSG, &SerialPanel::UpdateMessage,_parent);
}

commSerial::~commSerial()
{ 
  disconnect();

  delete _workerThr;
}

void commSerial::connect(){
  wxCommandEvent event_new(UPD_STATUS);
  bool p = open_port();
  if(!p){
    event_new.SetString("Not available");
    wxQueueEvent(_parent, event_new.Clone());
    return;
  }
  
  event_new.SetString("Connected");
  wxQueueEvent(_parent, event_new.Clone());
  
  config();
  std::future<void> future = _exit_signal.get_future();
  _workerThr = new std::thread(&commSerial::readserial,this,std::move(future));
}

void commSerial::disconnect(){
  std::cout << "Disconnecting\n";
  
  if(_dev >= 0){_exit_signal.set_value();std::cout << "Exit signaled\n";}
  
  if(_workerThr!=NULL){_workerThr->join();}
  
  close(_dev);
  std::cout << "_dev closed\n";  
}

void commSerial::readserial(std::future<void> future){
  while(future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout){
    memset(_buff, 0x00 , sizeof(_buff));
    int n = read(_dev, &_buff, sizeof(_buff));
    
    if(n<0){
      std::cout << "ERROR" << n << " " << strerror(n) << "\n";
      break;
    }
    
    if(n>0){
      wxGetApp()._msgHandler->msgpipe(_buff);
      std::cout << n << " : " << _buff << "\n";
    }
  }
}

bool commSerial::open_port(){
  
  int dev;
  char port[] = "/dev/ttyUSB0";
  
  dev = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  
  if (dev < 0){
    std::cout << "commSerial: "<< port << " " << strerror(dev) << "\n";
    return false;
  }
  
  _dev = dev;
  fcntl(dev,F_SETFL, 0);
  return true;
}

void commSerial::config(){
  tcgetattr(_dev, &_settings);
  cfsetispeed(&_settings, B115200);
  cfsetospeed(&_settings, B115200);
  
  _settings.c_cflag |= ( CLOCAL | CREAD );
  _settings.c_oflag &= ~OPOST;
  _settings.c_oflag &= ~ONLCR;
  _settings.c_cflag &= ~PARENB;
  _settings.c_cflag &= ~CSTOPB;
  _settings.c_cflag &= ~CRTSCTS;
  _settings.c_lflag &= ~ICANON;
  _settings.c_lflag &= ~ISIG;
  _settings.c_iflag &= ~( IXON | IXOFF | IXANY );
  _settings.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
  _settings.c_cc[VTIME] = 10;
  _settings.c_cc[VMIN] = 0;
  
  tcsetattr(_dev, TCSANOW, &_settings);
}
