#include "common.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

struct message
{
	string owner;
	string sender;
	string title;
	string messageText;
	string passphrase;
};

message recordMessage(string owner, string sender, string titleText, string messageText, string passphrase)
{
	message mes;
	mes.owner = owner;
	mes.sender = sender;
	mes.title = titleText;
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
	while (sqlite3_step(statement) == SQLITE_ROW) {
		const unsigned char* owner;
		const unsigned char* sender;
		const unsigned char* title;
		const unsigned char* message;
		const unsigned char* passphrase;
		owner = sqlite3_column_text(statement, 0);
		sender = sqlite3_column_text(statement, 1);
		title = sqlite3_column_text(statement, 2);
		message  = sqlite3_column_text(statement, 3);
		passphrase  = sqlite3_column_text(statement, 4);
		
		messagesVector.push_back(recordMessage((const char*)owner,(const char*)sender,(const char*)title,(const char*)message,(const char*)passphrase));
	}

	if (rc == SQLITE_ERROR) {
		fprintf(stderr, "failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_close(db);
	
	return(0);
}

void AccountManager::revealMessage(string message)
{
	printf("%s\n",(const char*)message.c_str());
}

void AccountManager::listMessages(string owner)
{
	getMessages(owner);
	int messageCount = messagesVector.size();
	printf("You have %d messages\n",messageCount);	
	printf("#:	FROM:	Title:\n");
	for (int i = 0; i < messageCount; i++)
	{
		printf("%d:	%s	%s\n",i,messagesVector[i].sender.c_str(),messagesVector[i].title.c_str());
	}
	cout << endl;
}

void AccountManager::option_readMessages() 
{
	int actionChoice;
	printf("Please enter message number:\n");
	cin >> actionChoice;
	
	string passphrase;
	message chosenMessage = messagesVector[actionChoice];
	printf("#:	FROM:	Title:\n");
	printf("%d:	%s	%s\n",actionChoice,chosenMessage.sender.c_str(),chosenMessage.title.c_str());
	printf("passphrase: \n");
	cin >> passphrase;
	
	if ( strcmp((const char*)passphrase.c_str(),(const char*)chosenMessage.passphrase.c_str()) == 0 ) 
	{
		AccountManager::revealMessage(chosenMessage.messageText);
		
	} else 
	{
		printf("Incorrect passphrase\n");
	}	
	
	AccountManager::promptOptions();
	
}

void AccountManager::option_writeMessage()
{
	string sender = (const char*)mngr.currentAccount;
	string sendTo;
	string title;
	string passphrase;
	string message;
	printf("TO: ");
	cin >> sendTo;
	printf("TITLE: ");
	cin >> title;
	printf("Passphrase: ");
	cin >> passphrase;
	printf("Message: ");
	cin >> message;

	
	sqlite3 *db;
	int rc;
	rc = sqlite3_open("gee.db", &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "failed to open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}

	const char *create_sql = "INSERT INTO MESSAGES (OWNER,SENDER,TITLE,MESSAGE,PASSPHRASE) VALUES (?, ?, ?, ?, ?);";
	sqlite3_stmt *statement;

	rc = sqlite3_prepare_v2(db, create_sql, -1, &statement, NULL);
	rc = sqlite3_bind_text( statement, 1, sendTo.c_str(), sendTo.length(),SQLITE_STATIC);	
	rc = sqlite3_bind_text( statement, 2, sender.c_str(), sender.length(),SQLITE_STATIC);	
	rc = sqlite3_bind_text( statement, 3, title.c_str(), title.length(),SQLITE_STATIC);	
	rc = sqlite3_bind_text( statement, 4, passphrase.c_str(), passphrase.length(),SQLITE_STATIC);	
	rc = sqlite3_bind_text( statement, 5, message.c_str(), message.length(),SQLITE_STATIC);	
	
	if (rc != SQLITE_OK) {
		fprintf(stderr, "failed to prepare statement: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}

	rc = sqlite3_step(statement);
	
	if (rc == SQLITE_ERROR) {
		fprintf(stderr, "failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_close(db);

	printf("Message Sent\n");
	
}

void AccountManager::promptOptions() 
{
	int actionChoice;
	AccountManager::listMessages((const char*)mngr.currentAccount);
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
