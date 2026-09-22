#ifndef NETWORK_SCANNER_H
#define NETWORK_SCANNER_H

#include <string>

class NetworkScanner {
private:
    std::string targetIp;
    int startPort;
    int endPort;

public:
    NetworkScanner(const std::string& ip, int start, int end);
    void scan();

};

#endif