# DBServer

Multiuser proxy-server with connection to PostgreSQL database.
- **Build** <br>
  To automate the assembly, ***cmake*** is used, when you run it, you must specify the path to the ***libpq*** library to establish a connection with PostgreSQL.
- **Start** 
  <br>*./DBServer host port pghost pgport dbName login password* <br>
  - *host* - server ip; *port* - server port;
  - *pghost* - postgre server ip; *pgport* - postgre port
