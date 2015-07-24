int promptAirportInstructions();
Airport promptAirportDetails();
void printAirportDetails(Airport port);
void printAllAirports(FILE *fPtr);
void moveAirportsCursor(FILE *fPtr, int pos);
Airport readFromAirportsFile(FILE *fPtr);
void writeToAirportsSeqFile(FILE *fPtr, Airport port);
void writeToAirportsFile(FILE *fPtr, Airport port);

void textAirportsFile(FILE *fPtr);
void newAirportRecord(FILE *fPtr);
void updateAirportRecord(FILE *fPtr);
void deleteAirportRecord(FILE *fPtr);

int getNumberOfPlanes(Airport airport);

void processAirports();

void processAirports()
{
	FILE *fPtr;
	int choice = 0;
	
	if((fPtr = fopen("airports.dat", "rb+")) == NULL)
	{
		if((fPtr = fopen("airports.dat", "wb")) == NULL)
		{					
			printf("File could not be opened.\n");
		}
		else
		{	
			printf("\nNew file created.\n");
		}
	}
	else
	{
		while((choice = promptAirportInstructions()) != 0)
		{
			switch(choice)
			{
				case 1:
					textAirportsFile(fPtr);
					break;
				case 2:
					newAirportRecord(fPtr);
					break;
				case 3:
					updateAirportRecord(fPtr);
					break;
				case 4:
					deleteAirportRecord(fPtr);
					break;
				case 5:
					printAllAirports(fPtr);
					break;
				default:
					printf("\nIncorrect choice.\n");
					break;				
			}
		}					
	}
	fclose(fPtr);
	
	printf("\nExiting...\n");	
}

int promptAirportInstructions()
{
	int option;
	printf("\n%s\n\n%s\n%s\n%s\n%s\n%s\n%s\n\n%s",
	"Please choose an option below to proceed.", 
	"1 - Create a text file for printing all records.",
	"2 - Create and save a new airport record.",
	"3 - Update an existing airport record.",
	"4 - Delete an existing airport record.", 
	"5 - View all airport records.", 
	"0 - Exit.",
	"Enter option: ");
	scanf("%d", &option);
	
	return option;	
}

Airport promptAirportDetails()
{
	Airport port = {0, "", "", "", 0, 0, {0}};
	printf("\nPlease enter airport details below.\n");
	
	printf("\nEnter airport name: ");
	fscanf(stdin, "%s", port.airportName);
	printf("\nEnter country: ");
	fscanf( stdin, "%s", port.country);
	printf("\nEnter usual weather: ");
	fscanf( stdin, "%s", port.usualWeather);
	printf("\nEnter time from Beirut(Hours): ");
	fscanf( stdin, "%d", &port.timeFromBeirut);
	printf("\nEnter miles from Beirut: ");
	fscanf( stdin, "%d", &port.milesFromBeirut);
	
	return port;
}

void printAirportDetails(Airport port)
{
	printf("\nID: %d", port.id);
	printf("\nAirport Name: %s", port.airportName);
	printf("\nCountry: %s", port.country);
	printf("\nUsual Weather: %s", port.usualWeather);
	printf("\nTime from Beirut(Hours): %d", port.timeFromBeirut);
	printf("\nMiles from Beirut: %d", port.milesFromBeirut);
	printf("\nNumber of planes: %d", getNumberOfPlanes(port));
	printf("\n\n");
}

void printAllAirports(FILE *fPtr)
{
	Airport port = {0, "", "", "", 0, 0, {0}};
		
	printf("\n%-5s%-10s%-15s%-10s%-9s%-7s%s\n", 
	"ID", "Name", "Country", "Weather", "Time", "Miles", "Planes");
	printf("%-5s%-10s%-15s%-10s%-9s%-7s%s\n",
	"--", "----", "-------", "-------", "----", "-----", "------");
	rewind(fPtr);
	while(!feof(fPtr))
	{
		port = readFromAirportsFile(fPtr);
		if(port.id != 0)
		{
			printf("\n%-5d", port.id);	
			printf("%-10s", port.airportName);
			printf("%-15s", port.country);		
			printf("%-10s", port.usualWeather);			
			printf("%-9d", port.timeFromBeirut);
			printf("%-7d", port.milesFromBeirut);
			printf("%d", getNumberOfPlanes(port));
		}		
	}
	
	printf("\n\n");
}

void moveAirportsCursor(FILE *fPtr, int pos)
{
	fseek(fPtr, ((pos - 1) * sizeof(Airport)), SEEK_SET);
}

Airport readFromAirportsFile(FILE *fPtr)
{
	Airport port = {0, "", "", "", 0, 0, {0}};
	fread(&port, sizeof(Airport), 1, fPtr);
	return port;
}

void writeToAirportsSeqFile(FILE *fPtr, Airport port)
{
	fprintf(fPtr, "%s, %s, %s, %d, %d, %d\n",
        port.airportName, port.country, port.usualWeather, 
		port.timeFromBeirut, port.milesFromBeirut, 
		getNumberOfPlanes(port)); 
}

void writeToAirportsFile(FILE *fPtr, Airport port)
{
	fwrite(&port, sizeof(Airport), 1, fPtr);
}

void textAirportsFile(FILE *readPtr)
{
	FILE *writePtr;
	
	Airport port = {0, "", "", "", 0, 0, {0}};
	
	if( (writePtr = fopen("airports.txt", "w")) == NULL )
	{
		printf("File could not be opened.\n");
	}
	else
	{
		rewind(readPtr);
		while(!feof(readPtr))
		{
			port = readFromAirportsFile(readPtr);
			if(port.id != 0)
			{
				writeToAirportsSeqFile(writePtr, port);
			}
		}
		fclose(writePtr);
	}
}

void updateAirportRecord(FILE *fPtr)
{
	int num;
	Airport port = {0, "", "", "", 0, 0, {0}};
	
	printf("Enter Airport ID to update: ");
	scanf("%d", &num);
	
	moveAirportsCursor(fPtr, num);
	port = readFromAirportsFile(fPtr);
	if(port.id == 0)
	{
		printf("No airport record found with airport ID, \"%d\".", num);
	}
	else
	{
		printAirportDetails(port);
		port = promptAirportDetails();
		port.id = num;
		moveAirportsCursor(fPtr, num);
		writeToAirportsFile(fPtr, port);		
		printf("\nUpdate successful.\n");
	}
}

void deleteAirportRecord(FILE *fPtr)
{
	Airport port;
	Airport emptyPort = {0, "", "", "", 0, 0, {0}};
	int num;
	
	printf("Enter Airport ID to delete: ");
	scanf("%d", &num);
	
	moveAirportsCursor(fPtr, num);
	port = readFromAirportsFile(fPtr);
	if(port.id == 0)
	{
		printf("No airport record with ID, %d.", num);
	}
	else
	{
		moveAirportsCursor(fPtr, num);
		writeToAirportsFile(fPtr, emptyPort);
		printf("\nDelete successful.\n");
	}
}

void newAirportRecord(FILE *fPtr)
{
	Airport port = {0, "", "", "", 0, 0, {0}};
	int id;
	
	printf("Enter new Airport ID: ");
	scanf("%d", &id);
	
	moveAirportsCursor(fPtr, id);
	port = readFromAirportsFile(fPtr);
	
	if(port.id != 0)
	{
		printf("Airport with ID \"%d\" contains information.\n", id);
	}
	else
	{
		port = promptAirportDetails();
		port.id = id;
		moveAirportsCursor(fPtr, id);
		writeToAirportsFile(fPtr, port);
		printf("\nInsertion successful.\n");
	}	
}

int getNumberOfPlanes(Airport port)
{
	int i, count;
	for(i = count = 0; i < MAX_PLANES; ++i)
	{
		if(port.planes[i] != 0)
		{
			++count;
		}
	}
	return count;
}









