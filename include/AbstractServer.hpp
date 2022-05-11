#ifndef ABSTRACTSERVER_HPP
#define ABSTRACTSERVER_HPP

# include <iostream>

# include <sys/types.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <sys/_types/_size_t.h>
# include <sys/socket.h>
# include <vector>
# include <fstream>
# include "../include/Session.hpp"
# include <ctime>
# include "DataAccessObject.hpp"

class AbstractServer {

public:

    AbstractServer(std::string host, std::string port);
    void                    start();
    virtual                 ~AbstractServer();

protected:

    virtual void            customConfiguration();
    virtual bool            conditionForSetWriteFD(const Session& session);
    virtual void            createSession(std::vector<Session *>&  serverSessions,
                                          int sessionSocket, struct sockaddr_in& addr);

    virtual int             serverWork_read(Session& session);
    virtual int             serverWork_write(Session& session);
    virtual void            serverWork_disconnect(Session& session) = 0;

private:

    void                    deleteSessions(std::vector<Session *> forDeletion);
    virtual void            check_sessionSocket();
    void                    maxFD();
    void                    init_fds();
    void                    createListening();
    addrinfo*               getListeningSocketConfig();
    void                    mainServerLoop();

    void                    do_select();
    void                    check_fds();
    void                    check_serverSocket();

    void                    zero_fd();
    void                    set_server_fd();
    void                    set_sessions_fd();


    std::string             host;
    std::string             port;
    std::vector<Session *> forDeletion;
    fd_set                  readfds;
    fd_set                  writefds;
    std::vector<Session *>  sessions;
    int                     serverSocket;
    size_t                  maxfd;


protected:

    void                    Getaddrinfo(const std::string& host, const std::string& port,
                                        addrinfo* hints, addrinfo** result);
    int                     Accept();
    int                     Select();
    void                    Listen(int listenQueue = 5);
    void                    Setsockopt();
    void                    Bind(const addrinfo& socketConfig);
    int                     Socket(int ai_family,int ai_socktype,int ai_protocol);
    void                    Error(const std::string& reason);
};


#endif
