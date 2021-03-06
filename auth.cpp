#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sqlite3.h>
#include <cstring>
using namespace std;


void AuthCenter::query_register(string username, string password) 
{
	//generate salt
	string salt = "tempSalt";
	//hash the password. salt the earth. board the windows

	sqlite3 *db;
	int rc;
	rc = sqlite3_open("gee.db", &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "failed to open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}

	const char *create_sql = "INSERT INTO USERS (NAME,PASSWORD,SALT) VALUES (?, ?, ?);";
	sqlite3_stmt *statement;

	rc = sqlite3_prepare_v2(db, create_sql, -1, &statement, NULL);
	rc = sqlite3_bind_text( statement, 1, username.c_str(), username.length(),SQLITE_STATIC);	
	rc = sqlite3_bind_text( statement, 2, password.c_str(), password.length(),SQLITE_STATIC);	
	rc = sqlite3_bind_text( statement, 3, salt.c_str(), salt.length(),SQLITE_STATIC);	
	
	if (rc != SQLITE_OK) {
		fprintf(stderr, "failed to prepare statement: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}

	rc = sqlite3_step(statement);
	
	if (rc == SQLITE_ERROR) {
		fprintf(stderr, "failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_close(db);

	printf("New account created\n");
}

/*
verifyUsername returns:
0: username does not exist
1: username does exist or error 
*/
int AuthCenter::verifyUsername(string username)
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
		fprintf(stderr, "failed to execute statement: %s\n", sqlite3_errmsg(db));
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

/*
verifyPasswordReg returns:
0: password does not meet requirements
1: password is OK
*/
int AuthCenter::verifyPasswordReg(string password)
{
	if (password.length() < 4) {
		printf("password must be more than 4 characters\n");
		return 0;
	} else {
		return 1;
	}
}

/*
verifyUsernameLogin returns:
0: username exists
1: username does not exist
*/
int AuthCenter::verifyUsernameLogin(string username)
{
	if (verifyUsername(username)) {
		return 0;
	} else {
		printf("user does not exist!\n");
		return 1;
	}
}

/*
verifyPasswordLogin returns:
0: password OK 
1: password incorrect or error
*/
int AuthCenter::verifyPasswordLogin(string username, string password)
{
	sqlite3 *db;
	int rc;
	rc = sqlite3_open("gee.db", &db);

	if (rc != SQLITE_OK) 
	{
		fprintf(stderr, "failed to open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	const char *fetch_sql = ("select password,name from users where name = ?");
	sqlite3_stmt *statement;

	rc = sqlite3_prepare_v2(db, fetch_sql, -1, &statement, NULL);
	rc = sqlite3_bind_text( statement, 1, username.c_str(), username.length(),SQLITE_STATIC);
	
	if (rc != SQLITE_OK) 
	{
		fprintf(stderr, "failed to prepare statement: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}

	rc = sqlite3_step(statement);
	const unsigned char *fetchedPass = sqlite3_column_text(statement, 0);
	const unsigned char* owner = sqlite3_column_text(statement, 1);
	
	if (rc == SQLITE_ERROR) 
	{
		fprintf(stderr, "failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_close(db);
	
	if ( strcmp((const char*)fetchedPass,(const char*)password.c_str()) == 0 ) 
	{
		//printf("Correct password\n");
		mngr.currentAccount = owner;
		return 0;
	} else 
	{
		printf("Incorrect Password\n");
		return 1;
	}	
}
