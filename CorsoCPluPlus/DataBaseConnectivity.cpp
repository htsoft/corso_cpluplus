#include <iostream>
#include <sql.h>
#include <sqlext.h>

void show_error(unsigned int handleType, const SQLHANDLE& handle) {
    SQLCHAR SQLState[1024];
    SQLCHAR message[1024];
    if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
        std::cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << std::endl;
}

struct Persona {
	SQLINTEGER id;
	SQLWCHAR name[64];
	SQLWCHAR surname[64];
};

int main() {
    SQLHANDLE sqlEnvHandle;
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlStmtHandle;
    SQLRETURN retCode = 0;
    Persona p;

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
            return -1;

        if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
            return -1;

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
            return -1;

        SQLCHAR retConString[1024];
        switch (SQLDriverConnect(sqlConnHandle, NULL, (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=Prova;UID=your_username;PWD=your_password;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
            case SQL_SUCCESS_WITH_INFO:
                show_error(SQL_HANDLE_DBC, sqlConnHandle);
                break;
            case SQL_INVALID_HANDLE:
            case SQL_ERROR:
                show_error(SQL_HANDLE_DBC, sqlConnHandle);
                retCode = -1;
                break;
            default:
                break;
        }

        if (retCode == -1)
            return -1;

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
            return -1;

        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT * FROM persona;", SQL_NTS)) {
            show_error(SQL_HANDLE_STMT, sqlStmtHandle);
            return -1;
        } else {
            while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
                SQLGetData(sqlStmtHandle, 1, SQL_C_ULONG, &p.id, 0, NULL);
                SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, &p.name, sizeof(p.name), NULL);
                SQLGetData(sqlStmtHandle, 3, SQL_C_CHAR, &p.surname, sizeof(p.surname), NULL);
                std::cout << "ID: " << p.id << " Name: " << p.name << " Surname: " << p.surname << std::endl;
            }
        }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

    return 0;
}
