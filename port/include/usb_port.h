#ifndef _USB_PORT_H_
#define _USB_PORT_H_

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/serial.h>
#include <string.h>
#include <vector>
#include <boost/thread.hpp>
#include <functional>  

namespace usb_port
{

// typedef void (*CallbackFun)(char* data);
typedef std::function<void(char*)> CallbackFun;  

class UsbPort
{
public:
    UsbPort();
    ~UsbPort();

    void setPortName(char* name);

    bool setBaudRate(const int baud_rate);

    bool openPort();
    void closePort();
    void clearPort();

    int  getBaudRate();
    int  getCFlagBaud(int baud_rate);

    int  writeData(char* data);

    boost::thread createReadCallback(CallbackFun function, int sleep_ms);
    void callbackRead(CallbackFun function, int sleep_ms);
    int  readPort(char *data);

    std::vector<int> getBaudRateList();

private:
    char* port_name_;
    int   socket_fd_;
    int   baud_rate_;
    int   max_data_length_;

    std::vector<int> baud_rate_list_;

    bool setupPort(const int cflag_baud);
};

}

#endif /* _USB_PORT_H_ */