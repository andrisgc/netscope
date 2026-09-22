#include "NetworkScanner.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== Scanner (C++) ===\n\n";

    string ip = "127.0.0.1";
    int startPort = 80;
    int endPort = 100;

    NetworkScanner scanner(ip, startPort, endPort);
    scanner.scan();

    return 0;
}