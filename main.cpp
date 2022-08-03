#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <mysql++/mysql++.h>

using std::cout;
using std::string;

void run_schema(mysqlpp::Connection &c) {
	
	std::ifstream schema("./own_schema.sql");
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
		
	mysqlpp::Connection c{"database", "domain:port", "username", "password"};
	if(c.connected() == true) { cout << "Connected to database" << "\n"; }
	run_schema(c);

	return 0;

}
