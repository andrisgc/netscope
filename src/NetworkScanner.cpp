#include "NetworkScanner.h"
#include <iostream>
#include <sys/socket.h> // Sockets POSIX
#include <arpa/inet.h> // Manipulação de IPs e estruturas sockaddr
#include <unistd.h>

using namespace std;

NetworkScanner::NetworkScanner(const string& ip, int start, int end)
    : targetIp(ip), startPort(start), endPort(end) {}

void NetworkScanner::scan() {
    cout << "[*] A iniciar varredura no alvo: " << targetIp << "\n";
    
    for (int port = startPort; port <= endPort; ++port) {

        // Pede ao kernel um descritor de rede
        // AF_INET -> Especifica qual o endereçamento IP será utilizado (no nosso caso IPv4)
        // SOCK_STREAM -> Especifica qual o protocolo de transporte será utilizado. STREAM = TCP | DGRAM = UDP
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            cerr << "[-] Erro ao criar socket na porta " << port << "\n";
            continue;
        }

        // Monta o endereço destino
        sockaddr_in targetAddr;
        targetAddr.sin_family = AF_INET;

        // (Host to Network Short) Converte o número da porta para a ordem de bytes da rede
        targetAddr.sin_port = htons(port);

        // (Pointer to Network) Converte o IP para binário
        inet_pton(AF_INET, targetIp.c_str(), &targetAddr.sin_addr);

        // Handshake TCP
        // (struct sockaddr*)&targetAddr -> Cast de sockaddr_in (IPv4) para sockaddr genérico
        int result = connect(sock, (struct sockaddr*)&targetAddr, sizeof(targetAddr));

        if (result == 0) {
            cout << "[+] Porta " << port << " - ABERTA\n";
        }

        close(sock);
    }

    cout << "[*] Varredura concluída.\n";
}