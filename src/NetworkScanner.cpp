#include "NetworkScanner.h"
#include <iostream>
#include <sys/socket.h> // Sockets POSIX
#include <arpa/inet.h> // Manipulação de IPs e estruturas sockaddr
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include <fcntl.h>

using namespace std;

// Mutex global para evitar que haja conflitos entre threads no cout
mutex coutMutex;

NetworkScanner::NetworkScanner(const string& ip, int start, int end)
    : targetIp(ip), startPort(start), endPort(end) {}

void NetworkScanner::scanRange(int start, int end) {
    for (int port = start; port <= end; ++port) {

        // Pede ao kernel um descritor de rede
        // AF_INET -> Especifica qual o endereçamento IP será utilizado (no nosso caso IPv4)
        // SOCK_STREAM -> Especifica qual o protocolo de transporte será utilizado. STREAM = TCP | DGRAM = UDP
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            cerr << "[-] Erro ao criar socket na porta " << port << "\n";
            continue;
        }

        // Monta o endereço destino
        sockaddr_in targetAddr{};
        targetAddr.sin_family = AF_INET;

        // (Host to Network Short) Converte o número da porta para a ordem de bytes da rede
        targetAddr.sin_port = htons(port);

        // (Pointer to Network) Converte o IP para binário
        inet_pton(AF_INET, targetIp.c_str(), &targetAddr.sin_addr);

        // Adiciona a flag não-bloqueante no socket
        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);

        // Handshake TCP
        // (struct sockaddr*)&targetAddr -> Cast de sockaddr_in (IPv4) para sockaddr genérico
        connect(sock, (struct sockaddr*)&targetAddr, sizeof(targetAddr));

        // Prepara o select() para monitorar o socket com timeout de 1 segundo
        fd_set fdWrite;
        FD_ZERO(&fdWrite);
        FD_SET(sock, &fdWrite);

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int result = -1;

        if (select(sock + 1, NULL, &fdWrite, NULL, &timeout) > 0) {
            int so_error;
            socklen_t len = sizeof(so_error);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

            if (so_error == 0) result = 0;
        }

        fcntl(sock, F_SETFL, flags);

        if (result == 0) {
            // Timeout de receção
            struct timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            // Configura o socket para ter limite de tempo no receive
            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

            // Prepara memória para receber a resposta
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));

            // Tenta ler o que o serviço tem a dizer
            int bytesRead = recv(sock, buffer, sizeof(buffer) - 1, 0);

            string banner = "";
            if (bytesRead > 0) {
                banner = string(buffer, bytesRead);

                // Pega apenas a primeira linha do texto
                size_t pos = banner.find('\n');
                if (pos != string::npos) banner = banner.substr(0, pos);
                if (!banner.empty() && banner.back() == '\r') banner.pop_back();
            }

            // lock_guard garante que apenas uma thread escreva no terminal por vez
            lock_guard<mutex> lock(coutMutex);
            std::cout << "[+] Porta " << port << " - ABERTA";
            if (!banner.empty())
                std::cout << " (Serviço: " << banner << ")";
            std::cout << "\n";
        }

        close(sock);
    }
}

void NetworkScanner::scan() {
    std::cout << "[*] A iniciar varredura multithread no alvo: " << targetIp << "\n";

    int totalPorts = (endPort - startPort) + 1;
    if (totalPorts <= 0) return;

    int numThreads = 4;
    if (totalPorts < numThreads)
        numThreads = totalPorts;

    int chunkSize = totalPorts / numThreads;
    vector<thread> threads;
    threads.reserve(numThreads);

    int currentStart = startPort;
    for (int i = 0; i < numThreads; ++i) {
        int currentEnd = (i == numThreads - 1) ? endPort : (currentStart + chunkSize - 1);

        threads.emplace_back(&NetworkScanner::scanRange, this, currentStart, currentEnd);
    
        currentStart = currentEnd + 1;
    }

    for (auto& t : threads)
        t.join();

    std::cout << "[*] Varredura multithread concluída.\n";
}