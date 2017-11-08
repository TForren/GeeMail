#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sqlite3.h>
using namespace std;

struct myData {
  string A;
  int B, C;
};

int exec_callback(void *ptr, int argc, char *argv[], char *names[])
{
  vector<myData> *list = reinterpret_cast<vector<myData> *>(ptr);
  myData d;
  d.A = argv[0] ? argv[0] : "";
  d.B = atoi(argv[1]);
  d.C = atoi(argv[2]);
  list->push_back(d);
  return 0;
}

void query_with_exec()
{
  vector<myData> list;
  sqlite3 *db;
  sqlite3_open("gee", &db);
  char *errmsg = NULL;
  sqlite3_exec(db, "SELECT a, b, c FROM SQList /* WHERE ... */", exec_callback, &list, &errmsg);
  if (errmsg) {
    printf("error: %s!\n", errmsg);
    return;
  }
}

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

int verifyUsername(string username)
{
  sqlite3 *db;
  sqlite3_stmt * stmt;
  char *errmsg = NULL;
  string query = "select from users where name = ('" + username + "');";
  if (sqlite3_open("gee.db",&db) == SQLITE_OK)
  {
    sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL ); 
    sqlite3_step( stmt );
    int id = sqlite3_column_int(stmt,0);
    const unsigned char *text = sqlite3_column_text(stmt,1);
    printf("%d  %s\n", id, text);
    if (sqlite3_column_text(stmt,1) == NULL )
    {
      printf("not taken\n");
    } else 
    {
      printf("taken\n");
    }
  } else {
    cout << "failed to open db\n";
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

int verifyPassword(string password)
{

}

void registration()
{
  printf("registration:\n");
  string username, password;
  printf("Enter username: \n");
  cin >> username;
  cin.clear();
  cin.ignore(10000,'\n');
  printf("Enter password: \n");
  cin >> password;

  //verify credentials
  verifyUsername(username);
  //query_register(username,password);
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

