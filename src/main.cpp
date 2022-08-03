#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <mysql++/mysql++.h>

using std::cout;
using std::cin;
using std::string;

char to_uppercase(char &in) {
	if(in >= 'a' && in <= 'z') {
		in -= ('a' - 'A');
	}
	return in;
}

void run_schema(mysqlpp::Connection &c) {
	
	std::ifstream schema("./src/schema.sql"); // because executable is in ../bin
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

string login(mysqlpp::Connection &c) {
	string new_session_token = "lmao token";
	return new_session_token;
}

int main() {
	
	mysqlpp::Connection c{"database", "domain:port", "username", "password"}; //change this
	if(c.connected() == true) { cout << "Connected to database" << "\n"; }
	run_schema(c);
	string session_token{""};

	char action;
	do {
		if(session_token.empty()) {
			do {
				cout << "(L)ogin, (Q)uit ";
				cin >> action;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				to_uppercase(action);
			} while(action != 'L' && action != 'Q');
			switch(action) {
				case 'L':
					session_token = login(c);
					break;
				// future proof
			}
		
		
		}
		else {
			cout << session_token << '\n';
		}


	} while( action != 'Q');

	return 0;

}
