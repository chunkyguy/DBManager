#include "DBManager.h"

#include <iostream>
#include <cassert>

namespace database {

#pragma mark - private functions
    namespace {
        int callback(void *result, int rowNo, char **rows, char **cols) {
            
            std::map<std::string, std::string> item;
            for (int i = 0; i < rowNo; ++i) {
                item[std::string(cols[i])] = std::string(rows[i]);
            }
            
            Result *r = static_cast<Result*>(result);
            r->push_back(item);
            
            //	std::vector<> resultArr;
            //	for(int i = 0; i < rowNo; ++i)	{
            //		std::map<> objectKeyDict;
            //		objectKeyDict = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:cols[i]],@"column",[NSString stringWithUTF8String:rows[i]],@"rows",nil];
            //		[resultArr addObject:objectKeyDict];
            //	}
            //	[sqlPool release];
            //	//NSLog(@"callback: %@",resultArr);
            //	[queryResult addObject:resultArr];
            //	//[];
            return 0;
        }
        
        
        bool Execute(const Object &slf, const std::string &query, Result *result) {
            return (sqlite3_exec(slf.object(), query.c_str(), callback, result, 0) == SQLITE_OK);
        }
    }

#pragma mark - Object
    Object::Object(const std::string &path)
    {
		bool res = (sqlite3_open(path.c_str(), &_object) == SQLITE_OK);
		assert(res);
	}
	
	Object::~Object()
    {
		sqlite3_close(_object);
	}

#pragma mark - Query
	Query::Query(const Object &database, const std::string &statement) :
	result_(),
	success_(true)
	{
		// Execute
		if (!Execute(database, statement, &result_)) {
			std::cout << "Query failed" << std::endl;
			success_ = false;
		}
		
		seek_ = result_.begin();
	}
	
	bool Query::End() const {
		return !(success_ && (seek_ != result_.end()));
	}
	
	std::map<std::string, std::string> Query::Get() const {
		return *seek_;
	}
	
	void Query::Next() {
		++seek_;
	}
}
/* EOF */
