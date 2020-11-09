#include <stdlib.h>
#include <stdio.h>
#include <mysql/mysql.h>

static char* host = "localhost";
static char* user = "root";
static char* pass = "password";
static char* dbname = "Estudiantes";

unsigned int port = 3306;
static char* sock = NULL;
unsigned int flag = 0;

int main(){
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;

    conn = mysql_init(NULL);
    
    if(!(mysql_real_connect(conn, host, user, pass, dbname, port, sock, flag))){
        fprintf(stderr, "\nError: %s [%d]\n", mysql_error(conn), mysql_errno(conn));
        exit(1);
    }
    printf("Conexion exitosa!\n");

    mysql_query(conn, "SELECT * FROM Alumnos;");
    res = mysql_store_result(conn);
    while(row = mysql_fetch_row(res)){
        printf("%s  %s  %s  %s  %s  %s\n", row[0], row[1], row[2], row[3], row[4], row[5]);
    }

    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}