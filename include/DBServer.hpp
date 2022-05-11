#ifndef SERVER_HPP
#define SERVER_HPP

# include "AbstractServer.hpp"

class DBServer : public AbstractServer {

public:

    DBServer(std::string host, std::string port,
                   std::string& pghost, std::string& pgport,
                   std::string& dbName, std::string& login, std::string& password);
    ~DBServer() override;

protected:
    int             serverWork_read(Session& session) override;
    int             serverWork_write(Session& session) override;
    void            serverWork_disconnect(Session& session) override;

    bool            conditionForSetWriteFD(const Session &session) override;

    DataAccessObject dao;
};


#endif
