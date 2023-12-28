
// Simple console serial terminal

#include <igris/getopt/cliopts.h>
#include <igris/util/dstring.h>
#include <iostream>
#include <nos/fprint.h>
#include <nos/io/serial_port.h>
#include <nos/print.h>
#include <string>
#include <thread>
#include <vector>

int main(int argc, char **argv)
{
    igris::cliopts cli;

    cli.add_string("port", 'p', "/dev/ttyS0");
    cli.add_string("baudrate", 'b', "115200");
    cli.add_string("parity", 'P', "N");
    cli.add_string("stopbits", 's', "1");
    cli.add_string("databits", 'd', "8");
    cli.parse(argc, argv);

    auto port = cli.get_string("port").unwrap();
    auto baudrate_str = cli.get_string("baudrate").unwrap();
    auto parity_str = cli.get_string("parity").unwrap();
    auto stopbits_str = cli.get_string("stopbits").unwrap();
    auto databits_str = cli.get_string("databits").unwrap();

    nos::println("port:", port);
    nos::println("baudrate:", baudrate_str);
    nos::println("parity:", parity_str);
    nos::println("databits:", databits_str);
    nos::println("stopbits:", stopbits_str);

    auto baudrate = std::stoi(baudrate_str);
    auto parity = parity_str[0];
    auto stopbits = std::stoi(stopbits_str);
    auto databits = std::stoi(databits_str);

    nos::serial_port serial;
    int ret = serial.open(port.c_str(), baudrate, parity, stopbits, databits);

    if (ret < 0)
    {
        nos::println("error when serial openned");
    }

    std::thread listen_thread([&serial]() {
        char buf[128];
        while (1)
        {
            int sz = *serial.read(buf, 128);
            if (sz > 0)
            {
                std::cout.write(buf, sz);
                std::cout.flush();
            }
        }
    });

    while (1)
    {
        std::string str;
        std::getline(std::cin, str);
        str = str + "\r\n";
        nos::fprintln("[send: {}]", igris::dstring(str));
        serial.write(str.data(), str.size());
    }
}