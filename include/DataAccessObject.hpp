#ifndef SERVER_TEST_DATAACCESSOBJECT_HPP
#define SERVER_TEST_DATAACCESSOBJECT_HPP

#include "libpq-fe.h"
#include <iostream>
#include "Session.hpp"
class DataAccessObject {

    PGconn *conn;

public:
    DataAccessObject(std::string& pghost, std::string& pgport, std::string& dbName, std::string& login, std::string& password);
    std::string DBrequest(const std::string& req, Session& session);
    std::string readFromRes(Session& session);
    ~DataAccessObject();
};


#endif
