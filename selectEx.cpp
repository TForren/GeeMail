#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("gee.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "failed to open in memory database: %s\n", 
                sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    const char *create_sql = "select * from users";
    sqlite3_stmt *statement;

    rc = sqlite3_prepare_v2(db, create_sql, -1, &statement, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "failed to prepare statement: %s\n",
                sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

	rc = sqlite3_step(statement);
	int id                    = sqlite3_column_int (statement, 0);
	const unsigned char *name = sqlite3_column_text(statement, 1);
	
	printf("%d %s\n", id, name);

    if (rc == SQLITE_ERROR) {
        fprintf(stderr, 
                "failed to execute statement: %s\n", 
                sqlite3_errmsg(db));
    }

    sqlite3_close(db);

/*
    sqlite3 *db;
	sqlite3_stmt *stmt;
	const char *sql = "SELECT * FROM User";
	
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
		return 0;
	}
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id                    = sqlite3_column_int (stmt, 0);
		const unsigned char *name = sqlite3_column_text(stmt, 1);
		// ...
	}
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
	}
	sqlite3_finalize(stmt);
*/
	/*
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   //Open database
   rc = sqlite3_open("gee.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   //Create SQL statement
   sql = "SELECT * from users";

   //Execute SQL statement
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return 0;
   */
}