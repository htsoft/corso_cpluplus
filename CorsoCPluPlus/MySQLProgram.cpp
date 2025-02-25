#include <mysql/mysql.h>
#include <iostream>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server = "localhost";
    const char *user = "root";
    const char *password = "password"; // Sostituire con la password corretta
    const char *database = "prova";

    conn = mysql_init(NULL);

    if (conn == NULL) {
        std::cerr << "mysql_init() failed\n";
        return EXIT_FAILURE;
    }

    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    if (mysql_query(conn, "SELECT id, nome, cognome FROM persona")) {
        std::cerr << "SELECT * FROM persona failed. Error: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    res = mysql_store_result(conn);

    if (res == NULL) {
        std::cerr << "mysql_store_result() failed. Error: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    int num_fields = mysql_num_fields(res);

    while ((row = mysql_fetch_row(res))) {
        for(int i = 0; i < num_fields; i++) {
            std::cout << (row[i] ? row[i] : "NULL") << " ";
        }
        std::cout << std::endl;
    }

    mysql_free_result(res);

    mysql_close(conn);

    return EXIT_SUCCESS;
}
