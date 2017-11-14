using namespace std;

class AccountManager
{
	
	public:
		const unsigned char* currentAccount;
		void listMessages(string owner);
		
		void promptOptions();
		void option_readMessages();
		void option_writeMessage();
		
		int getMessages(string owner);
		void revealMessage(string message);
};