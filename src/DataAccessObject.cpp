#include "../include/DataAccessObject.hpp"

DataAccessObject::DataAccessObject(std::string& pghost, std::string& pgport, std::string& dbName, std::string& login, std::string& password) {
    conn = PQsetdbLogin(pghost.c_str(), pgport.c_str(), NULL, NULL, dbName.c_str(), login.c_str(), password.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cout << "Connection to database failed: " << PQerrorMessage(conn);
        PQfinish(conn);
        exit(1);
    }
}

std::string DataAccessObject::DBrequest(const std::string& req, Session& session) {
    session.res  = PQexec(conn, req.c_str());
    std::string resp = "";
    if (PQresultStatus(session.res) != PGRES_TUPLES_OK && PQresultStatus(session.res) != PGRES_COMMAND_OK) {
        free(session.res);
        return "BAD REQUEST!!!\n\r";
    }
    session.ncols = PQnfields(session.res);
    session.nrows = PQntuples(session.res);
    return resp;
}

std::string DataAccessObject::readFromRes(Session& session) {
    std::string resp;
    if (!session.readed) {
        for (int i = 0; i < session.ncols; ++i)
            resp += std::string(PQfname(session.res, i)) + " ";
        resp += "\n";
    }
    for (int j = 0; j < session.ncols; ++j)
        resp +=  std::string(PQgetvalue(session.res, session.readed, j)) + " ";
    resp += "\n";
    session.readed++;
    if (session.readed == session.nrows ) {
        session.readed = session.nrows = 0;
        free(session.res);
    }
    return resp;
}

DataAccessObject::~DataAccessObject() {
    PQfinish(conn);
}
