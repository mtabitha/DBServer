#include "../include/AbstractServer.hpp"

    AbstractServer::AbstractServer(std::string host, std::string port) : host(host), port(port){}

    AbstractServer::~AbstractServer() { deleteSessions(sessions); close(serverSocket); }

    void        AbstractServer::customConfiguration() {}

    void        AbstractServer::start() {
        createListening();
        customConfiguration();
        mainServerLoop();
    }

    void    AbstractServer::createSession(std::vector<Session *>& serverSessions,
                                          int sessionSocket, struct sockaddr_in& addr) {
        serverSessions.push_back(new Session(sessionSocket));
    }

    void        AbstractServer::mainServerLoop() {
        while (true) {
            init_fds();
            maxFD();
            do_select();
            check_fds();
        }
    }

    void        AbstractServer::init_fds() {
        zero_fd();
        set_server_fd();
        set_sessions_fd();
    }

    void        AbstractServer::maxFD() {
        maxfd = serverSocket;
        for (auto &session : sessions) {
            maxfd = session->socket > maxfd ? session->socket : maxfd;
        }
    }

    void        AbstractServer::do_select() {
        Select();
    }

    void        AbstractServer::check_fds() {
        check_serverSocket();
        check_sessionSocket();
    }

    void        AbstractServer::check_serverSocket() {
        if (FD_ISSET(serverSocket, &readfds)) {
            Accept();
        }
    }

    void        AbstractServer::check_sessionSocket() {

        for (auto &session : sessions) {
            if (FD_ISSET(session->socket, &readfds)) {
                serverWork_read(*session);
            }
            if (FD_ISSET(session->socket, &writefds)) {
                serverWork_write(*session);
            }
        }
        deleteSessions(forDeletion);
        forDeletion.clear();
    }

    int            AbstractServer::serverWork_read(Session& session) {
        int ret = recv(session.socket, session.buf_read, BUF_SIZE - 1, 0);
        if (ret == 0) {
            forDeletion.push_back(&session);
            serverWork_disconnect(session);
        }
        return ret;
    }

    int            AbstractServer::serverWork_write(Session& session) {
        int ret = send(session.socket, session.buf_write, strlen(session.buf_write), 0);
        return ret;
    }

    void        AbstractServer::deleteSessions(std::vector<Session *> forDeletion) {
        for (int i = 0; i < forDeletion.size(); ++i) {
            for (auto first = sessions.begin(); first !=  sessions.end(); ++first) {
                if (*first == forDeletion[i]) {
                    sessions.erase(first);
                    break;
                }
            }
            delete (forDeletion[i]);
        }
    }

void         AbstractServer::zero_fd() {
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
}

void        AbstractServer::set_server_fd() {
    FD_SET(serverSocket, &readfds);
}

void        AbstractServer::set_sessions_fd() {
    for (auto &session : sessions) {
        FD_SET(session->socket, &readfds);
        if (conditionForSetWriteFD(*session))
            FD_SET(session->socket, &writefds);
    }
}

bool        AbstractServer::conditionForSetWriteFD(const Session& session) {
    return strlen(session.buf_write);
}

void        AbstractServer::createListening() {
    addrinfo* socketConfig = getListeningSocketConfig( );
    serverSocket = Socket(
            socketConfig->ai_family,
            socketConfig->ai_socktype,
            socketConfig->ai_protocol);
    Setsockopt();
    Bind(*socketConfig);
    Listen();
    freeaddrinfo(socketConfig);
}

addrinfo*   AbstractServer::getListeningSocketConfig() {
    addrinfo* result;
    addrinfo hints{AI_PASSIVE, AF_UNSPEC, SOCK_STREAM};
    Getaddrinfo(host, port, &hints, &result);
    return result;
}

void        AbstractServer::Getaddrinfo(const std::string& host, const std::string& port,
                                addrinfo* hints, addrinfo** result) {
    if (getaddrinfo(host.c_str(), port.c_str(), hints, result) != 0) {
        Error("getaddrinfo error");
    }
}

    int         AbstractServer::Accept() {
        sockaddr_in addr;
        socklen_t   addr_size = sizeof(addr);

        int res = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&addr), &addr_size);
        if ( res == -1) {
            Error("accept error");
        }
        createSession(sessions, res, addr);
        return res;
    }

    int         AbstractServer::Select() {
        int res = select(maxfd + 1, &readfds, &writefds, nullptr, nullptr);
        if ( res == -1) {
            Error("select error");
        }
        return res;
    }

    void        AbstractServer::Listen(int listenQueue) {
        int res = listen(serverSocket, listenQueue);
        if ( res == -1) {
            Error("listen error");
        }
    }

    void        AbstractServer::Setsockopt() {
        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }

    void        AbstractServer::Bind(const addrinfo& socketConfig) {
        int res = bind(serverSocket, socketConfig.ai_addr, socketConfig.ai_addrlen);
        if ( res == -1) {
            Error("bind error");
        }
    }

    int         AbstractServer::Socket(int ai_family,int ai_socktype,int ai_protocol) {
        int res = socket(ai_family, ai_socktype, ai_protocol);
        if (res == -1) {
            Error("socket error");
        }
        return res;
    }

    void        AbstractServer::Error(const std::string& reason) {
        std::cerr << "Server " + host + ":" + port + " - " + reason + " " + strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

