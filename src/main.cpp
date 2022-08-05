#include <iostream>
#include <fstream>
#include <string>
#include <mysql++/mysql++.h>

class User {
	private:
		std::string session_token{""};
	public:
		std::string username, password{""};

		User(std::string token):session_token(token) {}

		std::string get_token() { return session_token; }

		void login(mysqlpp::Connection &c) { username = "input-usrname"; password = "input-pwd"; } // hash password

		void make_token() {
			if (!username.empty() && !password.empty()) { session_token = username + password; } //hash etc..
			else { std::cout << "ERROR: username or password isn't availible for creation of session_token \n"; }
		}
};

void run_schema(mysqlpp::Connection &c) {
	
	std::ifstream schema("./src/schema.sql"); // because running from base dir
	std::string query{""};

	while(schema.good()) {
		std::string tmp{""};
		std::getline(schema, tmp);
		query += tmp;
	}

	mysqlpp::Transaction transaction(c, mysqlpp::Transaction::serializable, mysqlpp::Transaction::session);
	mysqlpp::Query exec(c.query());
	exec.execute(query);
	transaction.commit();
	std::cout << "INFO: schema loaded \n";
	return;

}

int main() {

	mysqlpp::Connection c{"database", "domain:port", "username", "password"}; //change this
	if(c.connected()) { std::cout << "INFO: connected to database \n"; }
	run_schema(c);
	User user{""};

	char action;
	do {
		if(user.get_token().empty()) {
			do {
				std::cout << "(L)ogin, (Q)uit ";
				std::cin >> action;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					if(action >= 'a' && action <= 'z') {
						action -= ('a' - 'A');
					}
			} while(action != 'L' && action != 'Q');

			switch(action) {
				case 'L':
					user.login(c);
					user.make_token();
					std::cout << user.username << ", " << user.password << '\n';
					break;
			}
		} else {
			std::cout << user.get_token() << '\n';
			action = 'Q'; 
		}


	} while( action != 'Q');

	return 0;

}
