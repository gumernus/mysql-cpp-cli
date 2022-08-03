#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <tuple>
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

		void update_creds(string usr_name, string usr_pwd){ username = usr_name; password = usr_pwd; }

		string get_token() { return session_token; }

		void make_token() {
			if (!username.empty() && !password.empty()) { session_token = username + password; } //hash etc..
			else { cout << "username or password isn't availible for creation of session_token" << '\n'; }
		}
};

char to_uppercase(char &in) {
	if(in >= 'a' && in <= 'z') {
		in -= ('a' - 'A');
	}
	return in;
}

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

std::tuple<string, string> login(mysqlpp::Connection &c) {
	return {"username-test", ""};
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
				to_uppercase(action);
			} while(action != 'L' && action != 'Q');
			switch(action) {
				case 'L':
					auto [username, password] = login(c);
					user.update_creds(username, password);
					user.make_token();
					cout << user.username << ", " << user.password << '\n';
					break;
				// future proof
			}
		
		}
		else {
			cout << user.get_token() << '\n';
			action = 'Q'; 
		}


	} while( action != 'Q');

	return 0;

}
