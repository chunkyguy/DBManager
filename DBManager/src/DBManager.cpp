//
//  DBManager.cpp
//  DBManager
//
//  Created by Sid on 07/07/14.
//  Copyright (c) 2014 whackylabs. All rights reserved.
//

#include "DBManager.h"

#include <iostream>
#include <cassert>
#include <sqlite3.h>

namespace database {
    
#pragma mark - private functions
    namespace {
        int callback(void *result, int rowNo, char **rows, char **cols) {
            
            std::map<std::string, std::string> item;
            for (int i = 0; i < rowNo; ++i) {
                item[std::string(cols[i])] = std::string(rows[i]);
            }
            
            std::vector<std::map<std::string, std::string> > *r = static_cast<std::vector<std::map<std::string, std::string> >*>(result);
            r->push_back(item);
            
            return 0;
        }
        
        
        bool Execute(const Object &slf,
                     const std::string &query,
                     std::vector<std::map<std::string, std::string> > *result) {
            return (sqlite3_exec(slf.object(), query.c_str(), callback, result, 0) == SQLITE_OK);
        }
    }
    
#pragma mark - Object
    Object::Object(const std::string &path) {
        bool res = (sqlite3_open(path.c_str(), &_object) == SQLITE_OK);
        assert(res);
	}
	
	Object::~Object() {
        sqlite3_close(_object);
	}
    
#pragma mark - Query
	Query::Query(const Object &database, const std::string &statement) :
	result_(),
	success_(true) {
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
	
	std::map<std::string, std::string> Query::Result() const {
        return *seek_;
	}
	
	void Query::Next() {
		++seek_;
	}
}
/* EOF */
