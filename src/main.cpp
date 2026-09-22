#include "NetworkScanner.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "=== Scanner (C++) ===\n\n";

    if (argc != 4) {
        cout << "Modo de uso incorreto.\n";
        return 1;
    }

    string ip = argv[1];
    int startPort = stoi(argv[2]);
    int endPort = stoi(argv[3]);

    NetworkScanner scanner(ip, startPort, endPort);
    scanner.scan();

    return 0;
}