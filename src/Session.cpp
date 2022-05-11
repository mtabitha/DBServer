#include "../include/Session.hpp"

Session::~Session() {
    close(socket);
}

Session::Session(int socket) : socket(socket), inner(nullptr) {
    readed = nrows = 0;
    memset(buf_read, 0, BUF_SIZE);
    memset(buf_write, 0, BUF_SIZE);
}