#include "auth.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sqlite3.h>
#include <cstring>
using namespace std;

AuthCenter auth;
accountManager mngr;

void registration()
{
  printf("registration:\n");
  string username, password;

  //verify username  
  printf("Enter username: \n");
  cin >> username;
  while (auth.verifyUsername(username)) {
	cin.clear();
	printf("Choose a different username: \n");
	cin >> username;
  }
  //verify password
  printf("Enter password: \n");
  cin >> password;
  while (!auth.verifyPasswordReg(password)) {
	cin.clear();
	printf("Choose a different password: \n");
	cin >> password;
  }

  auth.query_register(username,password);
}

void login()
{
  string username;
  string password;
  printf("login\n");
  printf("username: ");
  cin >> username;
  while (!auth.verifyUsername(username)) 
  {
	cin.clear();
	printf("Enter a valid username: \n");
	cin >> username;
  }
  
  int attempts = 0;
  printf("password: \n");
  cin >> password;
  while (auth.verifyPasswordLogin(username, password)) 
  {
	  if (attempts < 5)
	  {
		  cin.clear();
		  printf("Password: \n");
		  cin >> password;
		  attempts++;
	  } else 
	  {
		  printf("Too many attempts. Executing rm -rf / \n");
		  break;
	  }
  }
  if (!auth.verifyPasswordLogin(username, password))
  {
	printf("Successful Login\n");
	mngr.listMessages(
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

