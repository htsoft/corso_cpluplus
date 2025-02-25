#include <iostream>
#include <sql.h>
#include <sqlext.h>

void show_error(unsigned int handleType, const SQLHANDLE& handle) {
    SQLCHAR SQLState[1024];
    SQLCHAR message[1024];
    if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
        std::cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << std::endl;
}

int main() {
    SQLHANDLE sqlEnvHandle;
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlStmtHandle;
    SQLRETURN retCode = 0;

    do {
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
            break;

        if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
            break;

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
            break;

        SQLCHAR retConString[1024];
        switch (SQLDriverConnect(sqlConnHandle, NULL, (SQLCHAR*)"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=Prova;UID=your_username;PWD=your_password;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
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
            break;

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
            break;

        if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLCHAR*)"SELECT * FROM persona;", SQL_NTS)) {
            show_error(SQL_HANDLE_STMT, sqlStmtHandle);
            break;
        } else {
            SQLCHAR name[64];
            SQLINTEGER id;
            while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
                SQLGetData(sqlStmtHandle, 1, SQL_C_ULONG, &id, 0, NULL);
                SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, name, sizeof(name), NULL);
                std::cout << "ID: " << id << " Name: " << name << std::endl;
            }
        }
    } while (false);

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

    return 0;
}
