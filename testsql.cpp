#include <stdio.h>
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


int main(int argc, char* argv[]) 
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  const char *sql;
  const char* data = "Callback called";

  rc = sqlite3_open("gee.db", &db);

  if ( rc )
  {
    fprintf(stderr, "can't open Database: %s \n", sqlite3_errmsg(db));
    return(0);
  } else {
    fprintf(stderr, "opened database successfully\n");
  }
  sqlite3_close(db);

  sql = "SELECT * from users";

  rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

  if ( rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Operations done successfully\n");
  }
  sqlite3_close(db);
  return 0;

}
