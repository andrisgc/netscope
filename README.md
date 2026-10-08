<h1 align="center">
  <img src="docs/assets/n2z/png/netscope-c2-lockup-dark-2000.png" alt="Netscope Logo" width="600"><br>
  Netscope
</h1>

<p align="center">
  <b>Um Network Scanner veloz, assíncrono e multithreaded construído em C++ moderno.</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" alt="C++17">
  <img src="https://img.shields.io/badge/OS-Linux-orange.svg" alt="Linux">
</p>

---

## 📌 Sobre o Projeto

O **Netscope** é um scanner de portas de rede (mini-Nmap) desenvolvido do zero utilizando **POSIX Sockets**. Projetado para aliar a proximidade ao hardware do C++ com conceitos modernos de concorrência, o Netscope varre milhares de portas em segundos dividindo a carga de rede através de múltiplas *threads*.

## ✨ Funcionalidades

* **Alta Performance (Multithreading):** Divisão inteligente do intervalo de portas através de um *pool* dinâmico de `std::thread`.
* **Segurança de Concorrência:** Implementação rigorosa de `std::mutex` e `std::lock_guard` (RAII) para evitar *Race Conditions*.
* **Banner Grabbing Passivo:** Capacidade de ler *buffers* de resposta e identificar qual serviço está a rodar na porta.
* **Validação Robusta:** Blindagem de *inputs* e verificação de limites do protocolo TCP/IP.
* **Timeouts Customizados:** Utilização de `setsockopt` para evitar bloqueios infinitos.

## 🛠️ Tecnologias Utilizadas

* **Linguagem:** C++17
* **Rede:** Sockets POSIX nativos do Linux (`sys/socket.h`, `arpa/inet.h`)
* **Build System:** Makefile (Transição planeada para CMake)
* **Controle de Versão:** Git (com padrão Conventional Commits)

## 🚀 Como Executar

### Pré-requisitos
O projeto foi desenhado para ambientes baseados em Unix. Certifique-se de ter o compilador G++ e o utilitário Make instalados:

```bash
sudo apt update
sudo apt install build-essential
```

### Instalação e Compilação

1. Clone o repositório:
```bash
git clone https://github.com/andrisgc/netscope.git
cd netscope
```

2. Compile o código com o Makefile:
```bash
make
```

### Utilização

O Netscope exige três argumentos obrigatórios: IP, Porta Inicial e Porta Final.

**Sintaxe:**
```bash
./scanner <IP_ALVO> <PORTA_INICIAL> <PORTA_FINAL>
```

**Exemplo Prático:**
```bash
./scanner 192.168.0.1 1 1000
```

## 🧠 Arquitetura e Estrutura

O código-fonte segue as melhores práticas de Orientação a Objetos (OOP) no C++:

* `src/main.cpp`: Ponto de entrada, responsável por *parsing* de argumentos, validações (Fail-Fast) e instanciar o scanner.
* `src/NetworkScanner.h` e `src/NetworkScanner.cpp`: O coração do Netscope. Encapsulam as regras de divisão matemática das *threads*, manipulação de Sockets e extração de *Banners*.

## 🗺️ Roadmap (Próximos Passos)

* [ ] Migração de Makefile para **CMake** (Out-of-source builds).
* [ ] Implementação de Sockets Não-Bloqueantes com `fcntl` e `select()` para evasão de Firewalls restritivos.
* [ ] Exportação de relatórios em JSON.
