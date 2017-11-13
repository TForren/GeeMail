#include "common.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <vector>
using namespace std;

struct message
{
	string owner;
	string sender;
	string messageText;
	string passphrase;
};

message recordMessage(string owner, string sender, string messageText, string passphrase)
{
	message mes;
	mes.owner = owner;
	mes.sender = sender;
	mes.messageText = messageText;
	mes.passphrase = passphrase;
	return mes;
}

vector<message> messagesVector;

int AccountManager::getMessages(string owner)
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
	rc = sqlite3_bind_text( statement, 1, owner.c_str(),owner.length(),SQLITE_STATIC);
	
	if (rc != SQLITE_OK) {
		fprintf(stderr, "failed to prepare statement: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	/*
	while (1) {
		rc = sqlite3_step(statement);
			if (rc == SQLITE_ROW) {
				int owner;
				int sender;
				const unsigned char * message;
				const unsigned char * passphrase;
				owner = sqlite3_column_bytes(selectStmt, 0);
				sender = sqlite3_column_bytes(selectStmt, 1);
				message  = sqlite3_column_text(selectStmt, 2);
				passphrase  = sqlite3_column_text(selectStmt, 3);
			}
			else if (s == SQLITE_DONE) {
				break;
			}
	}	
	*/
	while (sqlite3_step(statement) == SQLITE_ROW) {
		const unsigned char* owner;
		const unsigned char* sender;
		const unsigned char * message;
		const unsigned char * passphrase;
		owner = sqlite3_column_text(statement, 0);
		sender = sqlite3_column_text(statement, 1);
		message  = sqlite3_column_text(statement, 2);
		passphrase  = sqlite3_column_text(statement, 3);
		
		messagesVector.push_back(recordMessage((const char*)owner,(const char*)sender,(const char*)message,(const char*)passphrase));
	}

	if (rc == SQLITE_ERROR) {
		fprintf(stderr, "failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_close(db);
	
	return(0);
}

void AccountManager::listMessages(string owner)
{
	getMessages(owner);
	
	for (int i = 0; i < messagesVector.size(); i++)
	{
		printf("%d: %s\n",i,messagesVector[i].sender.c_str());
	}
}

void AccountManager::option_readMessages() 
{
	
}

void AccountManager::option_writeMessage()
{
	
}

void AccountManager::promptOptions() 
{
	int actionChoice;
	printf("1: Read Messages\n2: Write Messages\n");
	cin >> actionChoice;

	if (actionChoice == 1)
	{
		option_readMessages();
	} else if (actionChoice == 2) 
	{
		option_writeMessage();
	} else 
	{
		printf("Enter a valid choice\n");
	}
}