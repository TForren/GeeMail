#include <iostream>
#include <stdio.h>
#include <string>
#include <termios.h>
#include <unistd.h>
using namespace std;

ssize_t getpass (char **lineptr, size_t *n, FILE *stream)
{
    struct termios old, new;
    int nread;

    /* Turn echoing off and fail if we can't. */
    if (tcgetattr (fileno (stream), &old) != 0)
        return -1;
    new = old;
    new.c_lflag &= ~ECHO;
    if (tcsetattr (fileno (stream), TCSAFLUSH, &new) != 0)
        return -1;

    /* Read the password. */
    nread = getline (lineptr, n, stream);

    /* Restore terminal. */
    (void) tcsetattr (fileno (stream), TCSAFLUSH, &old);

    return nread;
}


void login()
{
	string username;
	printf("login:\n");
	printf("username: ");
	cin >> username;
	string password = getpass("password: ", true);
	char pw[MAXPW] = {0};
	char *p = pw;
	FILE *fp = stdin;
	ssize_t nchr = 0;

	printf ( "\n Enter password: ");
	nchr = getpasswd (&p, MAXPW, '*', fp);
	printf ("\n you entered   : %s  (%zu chars)\n", p, nchr);

	printf ( "\n Enter password: ");
	nchr = getpasswd (&p, MAXPW, 0, fp);
	printf ("\n you entered   : %s  (%zu chars)\n\n", p, nchr);

}

void registration()
{
  printf("registration:\n");
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

