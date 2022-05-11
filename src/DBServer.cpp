#include "../include/DBServer.hpp"



    DBServer::DBServer(std::string host, std::string port,
                   std::string& pghost, std::string& pgport,
                   std::string& dbName, std::string& login, std::string& password) :

                   AbstractServer(host, port),
                   dao(pghost, pgport, dbName, login, password) {}

    DBServer::~DBServer() {}

    int        DBServer::serverWork_read(Session& session) {
        int res = AbstractServer::serverWork_read(session);
        if (res) {
            strcpy(session.buf_write, dao.DBrequest(std::string(session.buf_read), session).c_str());
            memset(session.buf_read, 0, strlen(session.buf_read));
        }
        return res;
    }

    int        DBServer::serverWork_write(Session& session) {
        if (session.nrows)
            strcpy(session.buf_write,  dao.readFromRes(session).c_str());
        int res = AbstractServer::serverWork_write(session);
        memset(session.buf_write, 0, strlen(session.buf_write));
        return res;
    }

    void        DBServer::serverWork_disconnect(Session& session) {
        std::cout << "Server work disconnect " << session.socket << std::endl;
    }

bool DBServer::conditionForSetWriteFD(const Session &session) {
    return AbstractServer::conditionForSetWriteFD(session) || session.nrows;
}
