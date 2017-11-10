#include <string>
using namespace std;

class AuthCenter
{
	public:
		int verifyUsername(string username);
		int verifyPasswordReg(string password);
		int verifyUsernameLogin(string username);
		int verifyPasswordLogin(string username, string password);
		void query_register(string username, string password);
};
