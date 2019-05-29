#include "../include/usb_port.h"

using namespace usb_port;

UsbPort::UsbPort()
    : max_data_length_(1024),
      socket_fd_(-1)
{

}

UsbPort::~UsbPort()
{
    clearPort();
    closePort();
}

void UsbPort::setPortName(char* name)
{
    port_name_ = name;
}

bool UsbPort::setBaudRate(const int baud_rate)
{
    if (baud_rate_ == baud_rate)
        return true;

    closePort();
    baud_rate_ = baud_rate;

    return setupPort(getCFlagBaud(baud_rate_));
}

bool UsbPort::openPort()
{
    return setupPort(getCFlagBaud(baud_rate_));
}

void UsbPort::closePort()
{
    if(socket_fd_ != -1)
        close(socket_fd_);
    socket_fd_ = -1;
}

void UsbPort::clearPort()
{
    tcflush(socket_fd_, TCIFLUSH);
}

int UsbPort::getBaudRate()
{
    return baud_rate_;
}

int UsbPort::writeData(char* data)
{
    return write(socket_fd_, data, max_data_length_);
}

boost::thread UsbPort::createReadCallback(CallbackFun function, int sleep_ms)
{
    boost::thread t = boost::thread(boost::bind(&UsbPort::callbackRead, this, function, sleep_ms));
    return t;
}

void UsbPort::callbackRead(CallbackFun function, int sleep_ms)
{
    while (true)
    {
        if (socket_fd_ != -1)
        {
            char data[max_data_length_];

            int result = readPort(data);
            if (result > 0)
            {
                function(data);
            }
                
            usleep(sleep_ms * 1000);
        }
    }
}

int UsbPort::readPort(char *data)
{
    return read(socket_fd_, data, max_data_length_);
}

std::vector<int> UsbPort::getBaudRateList()
{
    return baud_rate_list_;
}

bool UsbPort::setupPort(const int cflag_baud)
{
    struct termios newtio;

    socket_fd_ = open(port_name_, O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(socket_fd_ < 0)
    {
        printf("[PortHandlerLinux::SetupPort] Error opening serial port!\n");
        return false;
    }

    bzero(&newtio, sizeof(newtio)); // clear struct for new port settings

    newtio.c_cflag = cflag_baud | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag      = 0;
    newtio.c_lflag      = 0;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN]   = 0;

    // clean the buffer and activate the settings for the port
    tcflush(socket_fd_, TCIFLUSH);
    tcsetattr(socket_fd_, TCSANOW, &newtio);

    return true;
}

int UsbPort::getCFlagBaud(int baud_rate)
{
    switch (baud_rate)
    {
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;
        case 460800:
            return B460800;
        case 500000:
            return B500000;
        case 576000:
            return B576000;
        case 921600:
            return B921600;
        case 1000000:
            return B1000000;
        case 1152000:
            return B1152000;
        case 1500000:
            return B1500000;
        case 2000000:
            return B2000000;
        case 2500000:
            return B2500000;
        case 3000000:
            return B3000000;
        case 3500000:
            return B3500000;
        case 4000000:
            return B4000000;
        default:
            return -1;
    }
}
