#include <iostream>
#include <boost/thread.hpp>

#include "include/usb_port.h"

using namespace std;
using namespace usb_port;

class Test
{
public:
    Test() {}
    void run()
    {
        UsbPort *port = new UsbPort();

        port->setPortName("/dev/ttyUSB0");
        port->setBaudRate(115200);

        CallbackFun fun = bind(&Test::readData2, this, placeholders::_1);
        auto read_thread = port->createReadCallback(fun, 10);

        while (true)
        {
            // char data[1024];
            // printf("Pleas input data:");
            // scanf("%s", &data);
            // port->writeData(data);
            sleep(1);
        }
    }

private:
    void readData2(char* data)
    {
        printf("data: %s\n", data);
    }
};

void readData(char* data)
{
    printf("data: %s\n", data);
}

int main()
{
    // UsbPort *port = new UsbPort();

    // port->setPortName("/dev/ttyUSB0");
    // port->setBaudRate(115200);

    // auto read_thread = port->createReadCallback(readData, 10);

    // while (true)
    // {
    //     char data[1024];
    //     printf("Pleas input data:");
    //     scanf("%s", &data);
    //     port->writeData(data);
    //     sleep(1);
    // }
    

    // delete(port);
    Test t;
    t.run();
    return 0;
}