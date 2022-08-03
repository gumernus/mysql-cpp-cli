#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <mysql++/mysql++.h>

using std::cout;
using std::cin;
using std::string;

class User {
	private:
		string session_token{""};
	public:
		string username{""};
		string password{""};

		User(string token) { session_token = token; }
		string get_token() { return session_token; }

		void login(mysqlpp::Connection &c) { username = "input-usrname"; password = "input-pwd"; } // hash password

		void make_token() {
			if (!username.empty() && !password.empty()) { session_token = username + password; } //hash etc..
			else { cout << "username or password isn't availible for creation of session_token" << '\n'; }
		}
};

void run_schema(mysqlpp::Connection &c) {
	
	std::ifstream schema("./src/schema.sql"); // because running from base dir
	string query{""};

	while(schema.good()) {
		string tmp{""};
		std::getline(schema, tmp);
		query += tmp;
	}

	mysqlpp::Transaction transaction(c, mysqlpp::Transaction::serializable, mysqlpp::Transaction::session);
	mysqlpp::Query exec(c.query());
	exec.execute(query);
	transaction.commit();
	cout << "Schema loaded" << "\n";
	return;

}

int main() {
	
	mysqlpp::Connection c{"database", "domain:port", "username", "password"}; //change this
	if(c.connected() == true) { cout << "Connected to database" << "\n"; }
	run_schema(c);
	User user("");

	char action;
	do {
		if(user.get_token().empty()) {
			do {
				cout << "(L)ogin, (Q)uit ";
				cin >> action;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					if(action >= 'a' && action <= 'z') {
						action -= ('a' - 'A');
					}
			} while(action != 'L' && action != 'Q');

			switch(action) {
				case 'L':
					user.login(c);
					user.make_token();
					cout << user.username << ", " << user.password << '\n';
					break;
			}
		} else {
			cout << user.get_token() << '\n';
			action = 'Q'; 
		}


	} while( action != 'Q');

	return 0;

}
