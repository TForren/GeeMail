#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sqlite3.h>
#include <cstring>
using namespace std;

/*
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}
*/


void query_register(string username, string password ) 
{
  //generate salt
  string salt = "tempSalt";
  //hash the password. salt the earth. board the windows
  
  sqlite3 *db;
  sqlite3_stmt * stmt;
  char *errmsg = NULL;
  string query = "insert into users (name, password, salt) values ('" + username + "','" + password + "','" + salt + "');";
  if (sqlite3_open("gee.db",&db) == SQLITE_OK)
  {
    sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL ); 
    sqlite3_step( stmt );
  } else {
    cout << "failed to open db\n";
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

void login()
{
  string username;
  string password;
  printf("login\n");
  printf("username: ");
  cin >> username;
  printf("password: ");
  cin >> password;
}

int verifyUsernameReg(string username)
{
	sqlite3 *db;
	int rc;
	rc = sqlite3_open("gee.db", &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "failed to open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	const char *create_sql = ("select * from users where name = ?");
	sqlite3_stmt *statement;

	rc = sqlite3_prepare_v2(db, create_sql, -1, &statement, NULL);
	rc = sqlite3_bind_text( statement, 1, username.c_str(), username.length(),SQLITE_STATIC);
	
	if (rc != SQLITE_OK) {
		fprintf(stderr, "failed to prepare statement: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}

	rc = sqlite3_step(statement);
	//int id                    = sqlite3_column_int (statement, 0);
	const unsigned char *name = sqlite3_column_text(statement, 1);
	
	if (rc == SQLITE_ERROR) {
		fprintf(stderr, 
				"failed to execute statement: %s\n", 
				sqlite3_errmsg(db));
	}

	sqlite3_close(db);

	if (name == NULL) {
		//Not Taken
		printf("username available\n");
		return 0;
	} else {
		//Taken
		//printf("username unavailable\n");
		return 1;
	}
}

int verifyPasswordReg(string password)
{
	if (password.length() < 4) {
		printf("password must be more than 4 characters\n");
		return 1;
	} else {
		return 0;
	}
}

void registration()
{
  printf("registration:\n");
  string username, password;


  //verify username  
  printf("Enter username: \n");
  cin >> username;
  while (verifyUsernameReg(username)) {
	cin.clear();
	printf("Choose a different username: \n");
	cin >> username;
  }
  //verify password
  printf("Enter password: \n");
  cin >> password;
  while (verifyPasswordReg(password)) {
	cin.clear();
	printf("Choose a different password: \n");
	cin >> password;
  }

  if (!verifyUsernameReg(username) && !verifyPasswordReg(password)) {
	query_register(username,password);
  } else {
	printf("invalid username or password\n");  
  }
}

int main() 
{
	int loginChoice;
	printf("1: Login\n2: Register\n");
	cin >> loginChoice;

	if (loginChoice == 1)
	{
		login(); 
	} else if (loginChoice == 2) 
	{
		registration();
	} else 
	{
		printf("Enter a valid choice\n");
	}

}

