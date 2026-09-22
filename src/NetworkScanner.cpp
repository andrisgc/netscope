#include "NetworkScanner.h"
#include <iostream>

using namespace std;

NetworkScanner::NetworkScanner(const string& ip, int start, int end)
    : targetIp(ip), startPort(start), endPort(end) {}

void NetworkScanner::scan() {
    cout << "[*] A iniciar varredura no alvo: " << targetIp << "\n";
    cout << "[*] A testar portas: " << startPort << " - " << endPort << "\n";
    cout << "[+] Sockets\n";
}