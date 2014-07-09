#include <string>
#include <vector>
#include <map>
#include <sqlite3.h>

namespace database {

#pragma mark - Object
	/** A database object. Closes the database at destruction */
	class Object {
	public:
        /** path must be a valid absolute path to a database object */
		Object(const std::string &path);
		~Object();
		sqlite3 *object() const {return _object;}

	private:
        sqlite3 *_object;
        
        // No copy or assignment allowed
        Object(const Object&);
        void operator=(const Object&);
	};

#pragma mark - Query
    /** Query object.
     * Create all sort of queries and read the results in form of a loop like:
     *
     * for (database::Query q(&db, "select * from campaign"); !q.End(); q.Next()) {
     *      std::map<std::string, std::string> anEntry = q.Get();
     *      // process anEntry
     * }
     */
    class Query {
	public:
		Query(const Object &database, const std::string &statement);
		bool End() const;
		std::map<std::string, std::string> Result() const;
		void Next();
		
	private:
		std::vector<std::map<std::string, std::string> > result_;
		bool success_;
		std::vector<std::map<std::string, std::string> >::const_iterator seek_;
	};
	
}
