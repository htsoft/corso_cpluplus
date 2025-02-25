#include <iostream>
#include <windows.h>
#include <sqlext.h>

void show_error(unsigned int handletype, const SQLHANDLE& handle) {
    SQLCHAR SQLState[1024];
    SQLCHAR Message[1024];
    if (SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, SQLState, NULL, Message, 1024, NULL))
        std::cout << "SQL driver message: " << Message << "\nSQL state: " << SQLState << std::endl;
}

int main() {
    SQLHANDLE sqlenvhandle;
    SQLHANDLE sqlconnectionhandle;
    SQLHANDLE sqlstatementhandle;
    SQLRETURN retcode;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
        return -1;

    if (SQL_SUCCESS != SQLSetEnvAttr(sqlenvhandle, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0))
        return -1;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
        return -1;

    SQLCHAR retconstring[1024];
    switch (SQLDriverConnect(sqlconnectionhandle, NULL,
        (SQLWCHAR*)L"DRIVER={MySQL ODBC 8.0 Driver};SERVER=localhost;DATABASE=prova;USER=root;PASSWORD=your_password;",
        SQL_NTS, retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        std::cout << "Successfully connected to the database!" << std::endl;
        break;
    case SQL_INVALID_HANDLE:
    case SQL_ERROR:
        std::cout << "Could not connect to the database!" << std::endl;
        show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
        return -1;
    default:
        break;
    }

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle))
        return -1;
	// Execute SQL statement                                        SELECT * FROM persona;
    if (SQL_SUCCESS != SQLExecDirect(sqlstatementhandle, (SQLWCHAR*)L"SELECT id, nome, cognome FROM persona;", SQL_NTS)) {
        std::cout << "Error executing SQL statement!" << std::endl;
        show_error(SQL_HANDLE_STMT, sqlstatementhandle);
        return -1;
    }
    else {
        SQLINTEGER id;
        SQLCHAR nome[64], cognome[64];
        while (SQLFetch(sqlstatementhandle) == SQL_SUCCESS) {
            SQLGetData(sqlstatementhandle, 1, SQL_C_LONG, &id, 0, NULL);
            SQLGetData(sqlstatementhandle, 2, SQL_C_CHAR, nome, sizeof(nome), NULL);
            SQLGetData(sqlstatementhandle, 3, SQL_C_CHAR, cognome, sizeof(cognome), NULL);
            std::cout << "ID: " << id << " Nome: " << nome << " Cognome: " << cognome << std::endl;
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle);
    SQLDisconnect(sqlconnectionhandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);

    return 0;
}
