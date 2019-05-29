#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/serial.h>
#include <string.h>
#include <time.h>
#include <boost/thread.hpp>

using namespace std;

const char* port_name = "/dev/ttyUSB0";
const int baud_rate = B9600;
const int length = 8;

void setBaudrate(int port_fd, int baud_rate)
{
    struct termios newtio;

    bzero(&newtio, sizeof(newtio)); // clear struct for new port settings

    newtio.c_cflag = baud_rate | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag      = 0;
    newtio.c_lflag      = 0;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN]   = 0;

    // clean the buffer and activate the settings for the port
    tcflush(port_fd, TCIFLUSH);
    tcsetattr(port_fd, TCSANOW, &newtio);
}

void readCallback(int port_fd)
{
    char buf[100];  
    while (true)
    {
        char* packet;

        int result = read(port_fd, buf, 100);

        if (result > 0)
            printf("the buf is :%s, result %d\n", buf, result);

        usleep(100 * 1000);
    }
}

bool writeData(int port_fd, const char* data)
{
    int result = write(port_fd, data, strlen(data));
    if (result > 0)
    {
        printf("send success!\n");
    }
    else
    {
        printf("send failed!\n");
    }
    
}

int main()
{
    int port_fd = open(port_name, O_RDWR|O_CREAT, 0666);

    if(port_fd < 0)
    {
        printf("[PortHandlerLinux::SetupPort] Error opening serial port!\n");
    }

    // set_interface_attribs(port_fd, baud_rate);
    setBaudrate(port_fd, baud_rate);

    boost::thread read_thread;
    read_thread = boost::thread(&readCallback, port_fd);
    
    while (true)
    {
        char data[1024];
        printf("Pleas input data:");
        scanf("%s", &data);
        printf ("\n you input: %s\n", data);
        writeData(port_fd, data);
    }

    close(port_fd);
    return 0;
}