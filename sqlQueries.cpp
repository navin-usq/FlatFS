
int do_sql(int numLocal, char *attr, char *val) //sql function to create file or add spec
{
	sqlite3 *db;
  char *zErrMsg = 0;
  int rc,rc1,rc2;
  const char* sql;
	std::ostringstream query;
	std::string insert;
  rc = sqlite3_open(dataBaseLocation, &db); //path of the database to open connection
	if(numLocal == 0)
	{
		sql = "create table DataForFiles (Id key, Key text, Value text);"; //query to create table to store specs
		rc1 = sqlite3_exec(db, sql, NULL, NULL, NULL);
		query << "insert into DataForFiles values (" << num << ",'" << attr << "','" << val << "');"; //insert spec to database query
		insert = query.str();
		rc2 = sqlite3_exec(db, insert.c_str(), NULL, NULL, NULL); //execute query
		if( rc2 ) //check execution result
		{
      return(0);
   	}
	 	else
		{
			totalfilenums ++; //total number of records in dataforfiles
   	}
   sqlite3_close(db); //close connection
	 num++;
	 return (num - 1);
	}
	else
	{
		query << "insert into DataForFiles values (" << numLocal << ",'" << attr << "','" << val << "');"; //insert spec to database query
		insert = query.str();
		rc2 = sqlite3_exec(db, insert.c_str(), NULL, NULL, NULL);//execute query

  	if( rc2 ) //check execution result
		{
    	return(0);
  	}
		else
		{
			totalfilenums ++; //total number of records in dataforfiles
  	}
  	sqlite3_close(db); //close connection
		return 0;
	}
	return 0;
}

int do_sql_delete(int numLocal, char *attr, char *val) //sql function to delete spec
{
	sqlite3 *db;
  char *zErrMsg = 0;
  int rc,rc1,rc2;
  const char* sql;
	std::ostringstream query;
	std::string del;
  rc = sqlite3_open(dataBaseLocation, &db); //path of the database to open connection
	query << "delete from DataForFiles where Id = " << numLocal << " and Key like '" << attr << "' and Value like '" << val << "';";
	del = query.str();
	rc2 = sqlite3_exec(db, del.c_str(), NULL, NULL, NULL); //execute query
  if( rc2 ) //check execution result
	{
		std::cout << "delete error";
    return(0);
  }
	else
	{
		totalfilenums --; //total number of records in dataforfiles
  }
  sqlite3_close(db); //close connection
	return 0;
}
