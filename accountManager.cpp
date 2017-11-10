#include <stdio.h>
#include <stdlib.h>
using namespace std;

void AccountManager::listMessages(int id)
{
	sqlite3 *db;
	int rc;
	rc = sqlite3_open("gee.db", &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "failed to open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	const char *create_sql = ("select * from messages where owner = ?");
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
}

void AccountManager::readMessages() 
{
	
}

void AccountManager::writeMessage()
{
	
}

void AccountManager::promptOptions() 
{
	int actionChoice;
	printf("1: Read Messages\n2: Write Messages\n");
	cin >> loginChoice;

	if (actionChoice == 1)
	{
		ReadMessages();
	} else if (actionChoice == 2) 
	{
		writeMessage();
	} else 
	{
		printf("Enter a valid choice\n");
	}
}