#include <iostream>
#include <fstream>
#include <string>
#include <mysql++/mysql++.h>

class User {
	private:
		std::string session_token{""};
	public:
		std::string username, password{""};

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
	// will be in config file (down)
	std::cout << R""""(


	  __  __        _____  ____  _                                    _ _ 
	 |  \/  |      / ____|/ __ \| |                                  | (_)
	 | \  / |_   _| (___ | |  | | |  ______ ___ _ __  _ __ ______ ___| |_ 
	 | |\/| | | | |\___ \| |  | | | |______/ __| '_ \| '_ \______/ __| | |
	 | |  | | |_| |____) | |__| | |____   | (__| |_) | |_) |    | (__| | |
	 |_|  |_|\__, |_____/ \___\_\______|   \___| .__/| .__/      \___|_|_|
	          __/ |                            | |   | |                  
	         |___/                             |_|   |_|
	//////////////////////////////////////////////////////////////////////)"""";
	std::cout << '\n';
	return;

}

int main() {

	mysqlpp::Connection c{"database", "domain:port", "username", "password"}; //change this
	if(c.connected()) { run_schema(c); };
	User user;

	int action;
	do {
		if(user.get_token().empty()) {
			do {
				std::cout << "	1) Login \n";
				std::cout << "	2) Signin \n";
				std::cout << "	3) Quit \n";
				std::cout << "	//////////////////////////////////////////////////////////////////////\n";
				std::cout << "	: ";	
				std::cin >> action;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			} while(action != 1 && action != 2 && action != 3);

			switch(action) {
				case 1:
					user.login(c);
					user.make_token();
					std::cout << user.username << ", " << user.password << '\n';
					break;
				case 2:
					// Signin
					break;
			}
		} else {
			std::cout << user.get_token() << '\n';
			action = 3; 
		}


	} while( action != 3);

	return 0;

}
