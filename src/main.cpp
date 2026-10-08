#include "NetworkScanner.h"
#include <iostream>
#include <string>
#include <arpa/inet.h> // Para a validação do IP com o pton (Pointer to Network)

using namespace std;

bool isIpValid(const string& ip) {
    struct sockaddr_in sa;

    return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) == 1;
}

int main(int argc, char* argv[]) {
    cout << "=== Netscope (C++) ===\n\n";

    if (argc != 4) {
        cout << "Modo de uso incorreto.\n";
        return 1;
    }

    string ip = argv[1];

    if (!isIpValid(ip)) {
        cout << "[-] Erro: O endereço IP fornecido não é um endereço válido.\n";
        return 1;
    }

    int startPort = 0;
    int endPort = 0;

    try {
        startPort = stoi(argv[2]);
        endPort = stoi(argv[3]);
    } catch (const exception& e) {
        cout << "[-] Erro: As portas fornecidas devem ser números inteiros.\n";
        return 1;
    }

    if (startPort < 1 || endPort > 65535 || startPort > endPort) {
        cout << "[-] Erro: Intervalo de portas inválido.\n";
        return 1;
    }

    NetworkScanner scanner(ip, startPort, endPort);
    scanner.scan();

    return 0;
}