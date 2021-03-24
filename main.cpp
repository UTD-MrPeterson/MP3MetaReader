#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

using namespace std;

char readByte(int fd) {
    char readByte;
    int error = read(fd, &readByte, sizeof(char));
    if ( error < 0) {
        cerr << "Error reading: " << strerror(errno) << endl;
        exit(1);
    }
    return readByte;
}

int main(int argc, char** argv) {
    if ( argc < 2) {
        cerr << "Need a file argument!" << endl;
        exit(1);
    }
    const char* fileToOpen = argv[1];

    int fd = open(fileToOpen, O_RDONLY);
    if ( fd < 0) {
        cerr << "Unable to open file: " << strerror(errno) << endl;
        exit(1);
    }

    int bytesIn = 0;

    char firstByte = readByte(fd);
    char secondByte = readByte(fd);

    while ( ((firstByte  & 0xFF) != 0xFF) ||
            ((secondByte & 0xE0) != 0xE0)) {
        firstByte = secondByte;
        secondByte = readByte(fd);
        bytesIn++;
    }

    cout << "Found frame header after " << bytesIn << " bytes!" << endl;

    char thirdByte = readByte(fd);
    char fourthByte = readByte(fd);

    char versionMask = 0x18;

    int versionInfo = secondByte & versionMask;
    versionInfo = versionInfo >> 3;
    cout << "Version is " << versionInfo << endl;

    char layerMask = 0x06;
    int layer = secondByte & layerMask;
    layer = layer >> 1;
    cout << "Layer is " << layer << endl;

    char bitrateMask = 0xf0;
    int bitrate = thirdByte & bitrateMask;
    bitrate = bitrate >> 4;
    cout << "Bitrate is " << bitrate << endl;

    close(fd);

    return 0;
}
