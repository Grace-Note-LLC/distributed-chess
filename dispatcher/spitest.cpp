#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
    int fd;

    fd = open("/dev/spidev0.0", O_RDWR);
    // write(fd,  "\x37\xFF", 2);
    char buf[8] = "Hello\r\n";
    int bytes = 0;
    // bytes = read(fd, buf, sizeof(buf));
    bytes = write(fd, buf, sizeof(buf));

    // cout << "SPI READ: [" << buf << "] (" << bytes << " bytes)" << endl;
    cout << "Wrote " << bytes << " bytes!" << endl;

    // for (int i = 0; i < bytes; i++) {
        // cout << hex << (int)(buf[i]) << endl;
    // }

    close(fd);
}