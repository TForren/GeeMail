#include <iostream>
#include <stdio.h>
#include <string>
#include <termios.h>
#include <unistd.h>
using namespace std;

int getch() 
{
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

string getpass(const char *prompt, bool show_asterisk=true)
{
  const char BACKSPACE=127;
  const char RETURN=10;

  string password;
  unsigned char ch=0;

  cout << prompt << endl;

  while((ch=getch())!=RETURN)
  {
    if (ch==BACKSPACE)
    {
      if(password.length()!=0)
      {
        if(show_asterisk)
          cout << "\b \b";
          password.resize(password.length()-1);
      }
    } //if ch==backspace
    else 
    {
      password+=ch;
      if(show_asterisk)
        cout <<'*';
    }
  }
  cout << endl;
  return password;
}

void login()
{
  string username;
  printf("login:\n");
  printf("username: ");
  cin >> username;
  string password = getpass("password: ", true);
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

