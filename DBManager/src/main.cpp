#include <cassert>
#include <iostream>
#include <string>

#include "SF_FileSystem.h"
#include "DBManager.h"

/** 
 * database: master.db
 *
 * schema:
 *
 * CREATE TABLE campaign(id text primary key, score text, level text, ship_speed text);
 * CREATE TABLE player(campaign_id text);
 *
 * stored data in campaign:
 *
 * 0|950|1|50.0
 *
 * stored data in player:
 *
 * 0
 */
//const std::string kDBName("master.db");
//const std::string kTableName("campaign");

void print(const std::map<std::string, std::string> &map)
{
    for (std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " = " << it->second << std::endl;
    }
    std::cout << std::endl;
}

// Select operation
void testSelectQuery(const database::Object &db)
{
    std::map<std::string, std::string> expectedResult = {
        {"id", "0"},
        {"score", "950"},
        {"level", "1"},
        {"ship_speed", "50.0"}
    };
    
	for (database::Query q(db, "select * from campaign"); !q.End(); q.Next()) {
        assert(expectedResult == q.Get());
	}
}

// Insert operation
void testInsertQuery(const database::Object &db)
{
    std::map<std::string, std::string> expectedResults[2];
    expectedResults[0]["campaign_id"] = "0";
    expectedResults[1]["campaign_id"] = "4500";
    
	database::Query(db, "insert into player(campaign_id) values (4500)");

    int count = 0;
	for (database::Query q(db, "select * from player"); !q.End(); q.Next()) {
        assert(expectedResults[count++] == q.Get());
	}
}

//Update opertaion
void testUpdateQuery(const database::Object &db)
{
    std::map<std::string, std::string> expectedResult = {
        {"id", "0"},
        {"score", "1000"},
        {"level", "1"},
        {"ship_speed", "50.0"}
    };

    database::Query(db, "update campaign set score = '1000' where id = 0");

    for (database::Query q(db, "select * from campaign"); !q.End(); q.Next()) {
        assert(expectedResult == q.Get());
	}
}

void testDeleteQuery(const database::Object &db)
{
    std::map<std::string, std::string> expectedResult = {
        {"campaign_id", "4500"}
    };

    for (database::Query q(db, "delete from player where campaign_id = '0'"); !q.End(); q.Next()) {
        assert(expectedResult == q.Get());
    }
}

int main()
{
    // Resource file
    SF_File databaseFile("master", "db");
    
	// Database open
	database::Object db(databaseFile.GetPath());

    testSelectQuery(db);
    testInsertQuery(db);
    testUpdateQuery(db);
    testDeleteQuery(db);

	std::cout << "all tests passed" << std::endl;
	return 0;
}
