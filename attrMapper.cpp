char inMemoryNames[10024][100];

static int file_type(const char *path) //function to find type of user file command
{
	if(path[0] == '/' && path[1] == '?' && path[2] !=NULL) //file search command
	{
		return FILE_NAME;
	}
	/*else if(isFileFromCreate != 0) //file create command
	{std::cout << "inn2";
		if(isFileFromCreate == 2)
		{
			isFileFromCreate = 0;
		}
		else
			isFileFromCreate++;
		return FILE_REG;
	}
	else if(isFile == 1 && strcmp(path, "/?") != 0) //file create  command
	{std::cout << "inn3";
		int allowReg = 0;
		for(int i=0; i<strlen(path); i++)
		{
			if(path[i] == ':')
				allowReg = 1;
		}
		if(allowReg == 1)
		{
			fileReadCount++;
			if(fileReadCount == totalfiles)
				isFile = 0;
			return FILE_REG;
		}
		else
			FILE_NONE;
	}*/
	else if (strcmp(path, "/") == 0)//file list command from root
	{
		return FILE_ROOT;
	}
	else if(strcmp(path, "/?") == 0) //file list command
	{
		return FILE_LIST;
	}
	else if((path[0] == '/' && path[1] =='?') || move == 1) //query spec command
	{
		move = 1;
		return FILE_MOVE;
	}
	else if((path[0] == '/' && path[1] =='+') || move == 1) //add spec command
	{
		move = 1;
		return FILE_MOVE;
	}
	else if((path[0] == '/' && path[1] =='_') || move == 1) //delete spec command
	{
		move = 1;
		return FILE_MOVE;
	}
	else if((path[0] != NULL && path[1] == '<') || move == 1) //replace spec command
	{
		move = 1;
		return FILE_MOVE;
	}

	return FILE_NONE; //unknown command
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) //store databse value to struct object array
{ 
	NotUsed = 0;

	if(countglobal == 0) //allocate memory
	{
		memset(&s, 0, sizeof(s)); 
		s.argv1 = (char**)malloc((10500) * sizeof(char*));
		s.argv2 = (char**)malloc((10500) * sizeof(char*));
		s.argv3 = (char**)malloc((10500) * sizeof(char*));
	}

	int i=0;
	size_t n = 0;

	n = strlen(argv[0]) + 1; //store id
	s.argv1[countglobal] = (char*)malloc(n);
	strcpy(s.argv1[countglobal],argv[0]);

	n = strlen(argv[1]) + 1; //store key
	s.argv2[countglobal] = (char*)malloc(n);
	strcpy(s.argv2[countglobal],argv[1]);

	n = strlen(argv[2]) + 1; //store value
	s.argv3[countglobal] = (char*)malloc(n);
	strcpy(s.argv3[countglobal],argv[2]);

	countglobal ++;

  return 0;
}
