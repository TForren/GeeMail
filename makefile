all:
	g++ GeeMail.cpp auth.cpp AccountManager.cpp -lsqlite3 -o run.out
