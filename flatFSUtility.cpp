
//find files in the database
char inMemoryKey[10024][75];
char inMemoryValue[10024][75];
int inMemoryLength = 0;

//find files in the database
static int findFiles(const char *path, int isQuerySpec, int isFileExist)
{
			char **keyPath;
			char **valuePath;
			int searchCount = 1;

			for(int ii = 0; path[ii] != NULL; ii++) //count key value pairs
			{
				if(path[ii] == ',')
					searchCount++;
			}

			keyPath = (char**)malloc((searchCount + 1) * sizeof(char*)); //allocate memory for key,value pairs
			valuePath = (char**)malloc((searchCount + 1) * sizeof(char*));

			//variables to handle key,value pairs
			int keylimit = 0;
			int vallimit = 0;
			int comlimit = 0;
			int valRegion = 0;
			int nSize = 0;

			for(int ii = 1 + isQuerySpec; ii < strlen(path); ii++) //allocate memory for each key,value pair
			{
				if(path[ii] == ':')
				{
					keyPath[comlimit] = (char*)malloc(nSize);
					nSize = 0;
					valRegion = 1;
				}
				else if(path[ii] == ',' || path[ii + 1] == NULL)
				{
					if(path[ii + 1] == NULL)
						nSize++;
						valuePath[comlimit] = (char*)malloc(nSize);
						nSize = 0;
						valRegion = 0;
						comlimit++;
				}
				if(path[ii] != ':' && path[ii] != ',')
				{
					nSize++;
				}
			}

			comlimit = 0;

			for(int ii = 1 + isQuerySpec; ii < strlen(path); ii++) //separate key,value pairs and store it in array
			{
				if(path[ii] == ':')
				{
					valRegion = 1;
					keyPath[comlimit][keylimit] = NULL;
					keylimit = 0;
				}
				else if(path[ii] == ',')
				{
					valRegion = 0;
					valuePath[comlimit][vallimit] = NULL;
					comlimit++;
					vallimit = 0;
				}
				if((path[ii] != ':' && path[ii] != ',') && valRegion == 0)
				{
					keyPath[comlimit][keylimit] = path[ii];
					keylimit++;
				}
				else if((path[ii] != ':' && path[ii] != ',') && valRegion == 1)
				{
					valuePath[comlimit][vallimit] = path[ii];
					vallimit++;
				}
			}

			valuePath[comlimit][vallimit] = NULL;
			comlimit++;

			for(int i=0; i<totalfiles; i++)
			{
				int inMemoryCount = 1;
				for(int ii = 0; fileWithNames[i][ii] != NULL; ii++) //count key value pairs
				{
					if(fileWithNames[i][ii] == ',')
						inMemoryCount++;
				}

				int inMemoryKeyLength = 0;
				int inMemoryValueLength = 0;
				int iterationCount = 0;
				int w = 1;

				for(int k=0; k<inMemoryCount; k++)
				{
					while(fileWithNames[i][w]!=':')
					{
						inMemoryKey[inMemoryLength][inMemoryKeyLength] = fileWithNames[i][w];
						w++;
						inMemoryKeyLength++;
					}
					inMemoryKey[inMemoryLength][inMemoryKeyLength] = NULL;
					w++;
					while(fileWithNames[i][w] != ',' && fileWithNames[i][w] != NULL)
					{
						inMemoryValue[inMemoryLength][inMemoryValueLength] = fileWithNames[i][w];
						w++;
						inMemoryValueLength++;
					}
					inMemoryValue[inMemoryLength][inMemoryValueLength] = NULL;
					inMemoryLength++;
					w++;

					inMemoryKeyLength = 0;
					inMemoryValueLength = 0;
				}
//std::cout << path  << " " << inMemoryValue[0] << " " << inMemoryKey[0] << "\n";
				for(int ii=0; ii<inMemoryLength; ii++)
				{
						for(int ij = 0; ij < comlimit; ij++)
						{
							if((strcmp(keyPath[ij],inMemoryKey[ii]) == 0) && (strcmp(valuePath[ij],inMemoryValue[ii]) == 0))
							{
								iterationCount++;
							}
						}
				}

					//print start
				if(iterationCount == comlimit && isQuerySpec == 0 &&
				inMemoryLength == comlimit && isFileExist == 1) //file exist in database if condition is true
				{
					inMemoryLength = 0;
					return 1;
				}
				if(iterationCount == comlimit && isQuerySpec == 1 && isFileExist == 0) // save file in a array if it is a query spec
				{
					strncpy(fileNumsForMove[fileNumsForMoveCount], files[i] , 24);
					fileNumsForMoveCount++;
				}
				else if(iterationCount == comlimit && isQuerySpec == 0 && isFileExist == 0) // save file in a array if it is not a query spec
				{
					strncpy(fileNumsForMove[fileNumsForMoveCount], files[i] , 24);
				}
				inMemoryLength = 0;
			}

			return 0;
}

//get each file name for query spec
static int getFileNameForQuerySpec(int pathId)
{
	for(int i=0; i<strlen(querySpecFileName); i++) //clear memory
	{
		querySpecFileName[i] = NULL;
	}

	for(int i=0; i<totalfiles; i++) //get the corresponding file for query spec
	{
		if(strcmp(fileNumsForMove[pathId], files[i]) == 0)
		{
			strncpy(querySpecFileName, fileWithNames[i], 1024);
		char fname[100];
		int x=0;
		for(int wx=1;fileWithNames[i][wx]!=NULL;wx++)
		{
			fname[x] = fileWithNames[i][wx];
			x++;
		}
			fname[x] = NULL;
		strncpy(inMemoryNames[i], fname, 100);
		}
	}
}

//check file already exist for add spec
static int checkFileExistForAddSpec(const char *oldpath, const char *newpath, int pathId)
{
	char compareName[1024];
	int len = 0;
	compareName[len] = '/';
	len++;
	for(int ii = 1; oldpath[ii] != NULL; ii++) //find the file name
	{
		compareName[len] = oldpath[ii];
		len++;
	}
	compareName[len] = ',';
	len++;
	for(int ii = 2; newpath[ii] != '/'; ii++)
	{
		compareName[len] = newpath[ii];
		len++;
	}
	compareName[len] = NULL;

	if(findFiles(compareName, 0, 1) == 1) //check file exist
	{
		return 0;
	}

	for(int i=0; i<totalfiles; i++) //save the new file name for future use
	{
		if(strcmp(files[i], fileNumsForMove[pathId]) == 0)
		{
			strncpy(fileWithNames[i], compareName, 1024);
		char fname[100];
		int x=0;
		for(int wx=1;fileWithNames[i][wx]!=NULL;wx++)
		{
			fname[x] = fileWithNames[i][wx];
			x++;
		}
			fname[x] = NULL;
		strncpy(inMemoryNames[i], fname, 100);
			break;
		}
	}

	return 1;
}

//split key,value pairs to perform checks
static int splitKeyValuePair(const char *path)
{
	for(int j=0; j<splitKeyValuePairLimit; ++j) //clear memory
	{
		free(splitKeyPath[j]);
		splitKeyPath[j] = NULL;
		free(splitValuePath[j]);
		splitValuePath[j] = NULL;
	}
	splitKeyValuePairLimit = 0;
	int searchCount = 1;

	for(int ii = 0; path[ii] != NULL; ii++) //count number of key,value pairs
	{
		if(path[ii] == ',')
			searchCount++;
	}

	splitKeyPath = (char**)malloc((searchCount + 1) * sizeof(char*)); //alllocate memory
	splitValuePath = (char**)malloc((searchCount + 1) * sizeof(char*));

	int keylimit = 0;
	int vallimit = 0;
	int comlimit = 0;
	int valRegion = 0;
	int nSize = 0;

	for(int ii = 1; ii < strlen(path); ii++) //allocate additional memory
	{
		if(path[ii] == ':')
		{
			splitKeyPath[comlimit] = (char*)malloc(nSize);
			nSize = 0;
			valRegion = 1;
		}
		else if(path[ii] == ',' || path[ii + 1] == NULL)
		{
			if(path[ii + 1] == NULL)
				nSize++;
				splitValuePath[comlimit] = (char*)malloc(nSize);
				nSize = 0;
				valRegion = 0;
				comlimit++;
		}
		if(path[ii] != ':' && path[ii] != ',')
		{
			nSize++;
		}
	}

	comlimit = 0;

	for(int ii = 1; ii < strlen(path); ii++) //separate key,value pairs to corresponding arrays
	{
		if(path[ii] == ':')
		{
			valRegion = 1;
			splitKeyPath[comlimit][keylimit] = NULL;
			keylimit = 0;
		}
		else if(path[ii] == ',')
		{
			valRegion = 0;
			splitValuePath[comlimit][vallimit] = NULL;
			comlimit++;
			vallimit = 0;
		}
		if((path[ii] != ':' && path[ii] != ',') && valRegion == 0)
		{
			splitKeyPath[comlimit][keylimit] = path[ii];
			keylimit++;
		}
		else if((path[ii] != ':' && path[ii] != ',') && valRegion == 1)
		{
			splitValuePath[comlimit][vallimit] = path[ii];
			vallimit++;
		}
	}

	splitValuePath[comlimit][vallimit] = NULL;
	comlimit++;

	splitKeyValuePairLimit = comlimit;

	return 0;
}

//check spec exists in the file
static int checkSpecExistForAddSpec(const char *oldpath, const char *newpath)
{
	splitKeyValuePair(oldpath); //split key,value pairs

	char **keyPath;
	char **valuePath;
	int pathLimit;
	char **keyPathNew;
	char **valuePathNew;
	int pathNewLimit;

	keyPath = (char**)malloc((splitKeyValuePairLimit + 1) * sizeof(char*)); //allocate memory
	valuePath = (char**)malloc((splitKeyValuePairLimit + 1) * sizeof(char*));

	int nSize = 0;

	for(int t=0 ; t<splitKeyValuePairLimit ; t++) //allocate additional memory
	{
		for(int w=0; splitKeyPath[w] != NULL; w++)
		{
			nSize++;
		}
		keyPath[t] = (char*)malloc(nSize);
		nSize = 0;
	}

	for(int t=0 ; t<splitKeyValuePairLimit ; t++) //allocate additional memory
	{
		for(int w=0; splitValuePath[w] != NULL; w++)
		{
			nSize++;
		}
		valuePath[t] = (char*)malloc(nSize);
		nSize = 0;
	}

	for(int t=0 ; t<splitKeyValuePairLimit ; t++) //get the key,value pairs to local variables
	{
		int e=0;
		while(splitKeyPath[t][e] != NULL)
		{
			keyPath[t][e] = splitKeyPath[t][e];
			e++;
		}
		keyPath[t][e] = NULL;
		e=0;
		while(splitValuePath[t][e] != NULL)
		{
			valuePath[t][e] = splitValuePath[t][e];
			e++;
		}
		valuePath[t][e] = NULL;
	}
	pathLimit = splitKeyValuePairLimit;

	char trimNewPath[1024];
	int we=0;
	trimNewPath[we] = '/';
	we++;

	for(int r=2; newpath[r]!='/'; r++) //trim the new path
	{
		trimNewPath[we] = newpath[r];
		we++;
	}
	trimNewPath[we] = NULL;

	splitKeyValuePair(trimNewPath); // get key,value pairs of new path

	keyPathNew = (char**)malloc((splitKeyValuePairLimit + 1) * sizeof(char*)); //allocate memory
	valuePathNew = (char**)malloc((splitKeyValuePairLimit + 1) * sizeof(char*));

	nSize = 0;

	for(int t=0 ; t<splitKeyValuePairLimit ; t++) //allocate aditional memory
	{
		for(int w=0; splitKeyPath[w] != NULL; w++)
		{
			nSize++;
		}
		keyPathNew[t] = (char*)malloc(nSize);
		nSize = 0;
	}

	for(int t=0 ; t<splitKeyValuePairLimit ; t++)//allocate aditional memory
	{
		for(int w=0; splitValuePath[w] != NULL; w++)
		{
			nSize++;
		}
		valuePathNew[t] = (char*)malloc(nSize);
		nSize = 0;
	}

	for(int t=0 ; t<splitKeyValuePairLimit ; t++) //get the key,value pairs of new file to local variables
	{
		int e=0;
		while(splitKeyPath[t][e] != NULL)
		{
			keyPathNew[t][e] = splitKeyPath[t][e];
			e++;
		}
		keyPathNew[t][e] = NULL;
		e=0;
		while(splitValuePath[t][e] != NULL)
		{
			valuePathNew[t][e] = splitValuePath[t][e];
			e++;
		}
		valuePathNew[t][e] = NULL;
	}
	pathNewLimit = splitKeyValuePairLimit;

	for(int r=0; r<pathLimit; r++) //compare key,value pairs and return result
	{
		for(int re=0; re<pathNewLimit; re++)
		{
			if(strcmp(keyPath[r],keyPathNew[re]) == 0 && strcmp(valuePath[r], valuePathNew[re]) == 0)
			{
				return 0;
			}
		}
	}

	return 1;
}

//conditions to be performed before a delete of spec
static int checkFileExistForDeleteSpec(const char *path, const char *deletePath, int pathId)
{
	splitKeyValuePair(path); //split key value pairs

	char **keyPath;
	char **valuePath;
	int pathLimit;
	char pathToSend[1024];
	char **keyPathDelete;
	char **valuePathDelete;
	int pathDeleteLimit;

	keyPath = (char**)malloc((splitKeyValuePairLimit + 1) * sizeof(char*)); //allocate memory
	valuePath = (char**)malloc((splitKeyValuePairLimit + 1) * sizeof(char*));

	int nSize = 0;

	for(int t=0 ; t<splitKeyValuePairLimit ; t++) //allocate additional memory
	{
		for(int w=0; splitKeyPath[w] != NULL; w++)
		{
			nSize++;
		}
		keyPath[t] = (char*)malloc(nSize);
		nSize = 0;
	}

	for(int t=0 ; t<splitKeyValuePairLimit ; t++)//allocate additional memory
	{
		for(int w=0; splitValuePath[w] != NULL; w++)
		{
			nSize++;
		}
		valuePath[t] = (char*)malloc(nSize);
		nSize = 0;
	}


	for(int t=0 ; t<splitKeyValuePairLimit ; t++) //get key,value pairs locally
	{
		int e=0;
		while(splitKeyPath[t][e] != NULL)
		{
			keyPath[t][e] = splitKeyPath[t][e];
			e++;
		}
		keyPath[t][e] = NULL;
		e=0;
		while(splitValuePath[t][e] != NULL)
		{
			valuePath[t][e] = splitValuePath[t][e];
			e++;
		}
		valuePath[t][e] = NULL;
	}
	pathLimit = splitKeyValuePairLimit;

	int eee =0;
	for(int i=0; deletePath[i+1] != '/'; i++)
	{
		pathToSend[i] = deletePath[i+1];
		eee++;
	}
	pathToSend[eee] = NULL;

	splitKeyValuePair(pathToSend); //split key,value pairs of new path

	keyPathDelete = (char**)malloc((splitKeyValuePairLimit + 1) * sizeof(char*)); //allocate memory
	valuePathDelete = (char**)malloc((splitKeyValuePairLimit + 1) * sizeof(char*));

	for(int t=0 ; t<splitKeyValuePairLimit ; t++) //allocate additional memory
	{
		for(int w=0; splitKeyPath[w] != NULL; w++)
		{
			nSize++;
		}
		keyPathDelete[t] = (char*)malloc(nSize);
		nSize = 0;
	}

	for(int t=0 ; t<splitKeyValuePairLimit ; t++)//allocate additional memory
	{
		for(int w=0; splitValuePath[w] != NULL; w++)
		{
			nSize++;
		}
		valuePathDelete[t] = (char*)malloc(nSize);
		nSize = 0;
	}

	for(int t=0 ; t<splitKeyValuePairLimit ; t++) //get key,value pairs locally
	{
		int e=0;
		while(splitKeyPath[t][e] != NULL)
		{
			keyPathDelete[t][e] = splitKeyPath[t][e];
			e++;
		}
		keyPathDelete[t][e] = NULL;
		e=0;
		while(splitValuePath[t][e] != NULL)
		{
			valuePathDelete[t][e] = splitValuePath[t][e];
			e++;
		}
		valuePathDelete[t][e] = NULL;
	}

	pathDeleteLimit = splitKeyValuePairLimit;

	int iterationCount = 0;

	for(int i=0; i<pathDeleteLimit; i++) //compare key,value pair and return results
	{
		for(int j=0; j<pathLimit; j++)
		{
			if(strcmp(keyPathDelete[i], keyPath[j]) == 0 && strcmp(valuePathDelete[i], valuePath[j]) == 0)
			{
				iterationCount++;
			}
		}
	}

	if(iterationCount != pathDeleteLimit)
	{
		return 1;
	}
	else if(iterationCount == pathDeleteLimit && iterationCount == pathLimit)
	{
		return 2;
	}
	else
	{
		iterationCount = 0;
		char newPath[1024];
		int newSpecLimit = 0;

		newPath[newSpecLimit] = '/';
		newSpecLimit++;

		for(int i=0; i<pathLimit; i++)
		{
			for(int j=0; j<pathDeleteLimit; j++)
			{
				if(strcmp(keyPathDelete[j], keyPath[i]) == 0 && strcmp(valuePathDelete[j], valuePath[i]) == 0)
				{
					iterationCount++;
				}
			}
			if(iterationCount == 0)
			{
				if(newSpecLimit != 1)
				{
					newPath[newSpecLimit] = ',';
					newSpecLimit++;
				}
				int k=0;
				while(keyPath[i][k]!=NULL)
				{
					newPath[newSpecLimit] = keyPath[i][k];
					newSpecLimit++;
					k++;
				}
				newPath[newSpecLimit] = ':';
				newSpecLimit++;
				k=0;
				while(valuePath[i][k]!=NULL)
				{
					newPath[newSpecLimit] = valuePath[i][k];
					newSpecLimit++;
					k++;
				}
			}
			else
			{
				iterationCount = 0;
			}
		}

		newPath[newSpecLimit] = NULL;

		if(findFiles(newPath, 0, 1) == 1)
		{
			std::cout << newPath;
			return 3;
		}

		for(int i=0; i<totalfiles; i++)
		{
			if(strcmp(files[i], fileNumsForMove[pathId]) == 0)
			{
				strncpy(fileWithNames[i], newPath, 1024);
				char fname[100];
		int x=0;
		for(int wx=1;fileWithNames[i][wx]!=NULL;wx++)
		{
			fname[x] = fileWithNames[i][wx];
			x++;
		}
			fname[x] = NULL;
		strncpy(inMemoryNames[i], fname, 100);
				break;
			}
		}
	}

	return 0;
}

//add or delete file spec
static int addOrDeleteFileSpec(const char *path, int isDeleteSpec, int pathId)
{
	char fileName[1024];//initialize variables
	char attr[1024];
	char val[1024];
	int n=0,l=0,fileNum;
	for(int i=2; i<strlen(path); i++) //count key,value pairs to add or delete
	{
		if(path[i] != ':')
		{
			attr[n] = path[i];
			n++;
		}
		else
		break;
	}

	attr[n] = '\0';

	for(int i=n+3; path[i] != ',' ; i++)
	{
		if(path[i] != '/')
		{
		val[l] = path[i];
		l++;
	}
	else
	break;
	}

	val[l] = '\0';

	for(int i=0; i<totalfiles; i++) //get the required id of the key,value pair
	{
		if(strcmp(files[i], fileNumsForMove[pathId]) == 0)
		{
			fileNum = fileNums[i];
			break;
		}
	}

	if(isDeleteSpec == 0)
		do_sql(fileNum, attr, val); //perform add spec in database for first pair
	else
		do_sql_delete(fileNum, attr, val); //perform delete spec in database for first pair

	int w = n+l+3;

	while(path[w] != '/')//perform add or delete spec in database for other pairs
	{
		n=l=0;
		strcpy(attr,"");
		strcpy(val,"");
		for(int i=w+1; i<strlen(path); i++)
		{
			if(path[i] != ':')
			{
				attr[n] = path[i];
				n++;
			}
			else
			break;
		}

		attr[n] = '\0';

		for(int i=w+n+2; path[i] != ',' ; i++)
		{
			if(path[i] != '/')
			{
				val[l] = path[i];
				l++;
			}
			else
			break;
		}

		val[l] = '\0';

		if(isDeleteSpec == 0)
			do_sql(fileNum, attr, val); //perform add spec in database
		else
			do_sql_delete(fileNum, attr, val); //perform delete spec in database

		w = w+n+l+2;
	}
	return 0;
}
