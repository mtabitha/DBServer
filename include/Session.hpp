#ifndef SESSION_HPP
#define SESSION_HPP

#define BUF_SIZE 2048

#include <string>
#include <unistd.h>
#include "libpq-fe.h"
#include <sys/types.h>
#include <sys/socket.h>
struct Session {

    std::string address;
    std::string port;

    Session* inner;

    char    buf_read[BUF_SIZE];
    char    buf_write[BUF_SIZE];
    int     socket;

    PGresult* res;
    int      nrows;
    int      ncols;
    int      readed;


    Session(int socket);
    virtual ~Session();
};

#endif
