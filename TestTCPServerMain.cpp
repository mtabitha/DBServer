#include <iostream>

#include "include/DBServer.hpp"

int main(int argc, char *argv[]) {

    if (argc != 8) {
        std::cerr << "host port pghost pgport dbName login password " <<std::endl;
        return 1;
    }

    std::string host        = argv[1];
    std::string port        = argv[2];
    std::string pghost      = argv[3];
    std::string pgport      = argv[4];
    std::string dbName      = argv[5];
    std::string login       = argv[6];
    std::string password    = argv[7];


    DBServer proxyServer(host, port, pghost, pgport, dbName, login, password);

    proxyServer.start();

    return 0;
}
