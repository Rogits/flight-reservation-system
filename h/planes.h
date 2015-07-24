int promptPlaneInstructions();
Plane promptPlaneDetails();
void printPlaneDetails(Plane p);
void printAllPlanes(FILE *fPtr);

void movePlanesCursor(FILE *fPtr, int pos);
Plane readFromPlanesFile(FILE *fPtr);
void writeToPlanesSeqFile(FILE *fPtr, Plane p);
void writeToPlanesFile(FILE *fPtr, Plane p);

void textPlanesFile(FILE *fPtr);
void newPlaneRecord(FILE *fPtr);
void updatePlaneRecord(FILE *fPtr);
void deletePlaneRecord(FILE *fPtr);

int checkPlaneName(FILE *fPtr, char * desc);
int checkSeatNumber(Plane p, int num);
int getSeatNumberClass(Plane p, int seatNum);
int getTotalNumberOfFirstClassSeats(Plane p);
int getTotalNumberOfSecondClassSeats(Plane p);
int getTotalNumberOfReservedSeats(Plane p);
int getTotalNumberOfFirstClassReservedSeats(Plane p);
int getTotalNumberOfSecondClassReservedSeats(Plane p);
int getTotalNumberOfSeats(Plane p);

int reserveSeat(Plane p, int seatNum);
int unreserveSeat(Plane p, int seatNum);

int checkAirportId(int airportId);
void addPlaneToAirport(int planeId, int airportId);
void removePlaneFromAirport(int planeId, int airportId);

void processPlanes();

void processPlanes()
{
	FILE *fPtr;
	int choice = 0;
		
	if((fPtr = fopen("planes.dat", "rb+")) == NULL)
	{
		if((fPtr = fopen("planes.dat", "wb")) == NULL)
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
		while((choice = promptPlaneInstructions()) != 0)
		{
			switch(choice)
			{
				case 1:
					textPlanesFile(fPtr);
					break;
				case 2:
					newPlaneRecord(fPtr);
					break;
				case 3:
					updatePlaneRecord(fPtr);
					break;
				case 4:
					deletePlaneRecord(fPtr);
					break;
				case 5:
					printAllPlanes(fPtr);
					break;
				default:
					printf("Incorrect choice.\n");
					break;				
			}
		}		
		fclose(fPtr);		
		printf("\nExiting...\n");		
	}	
}

int checkPlaneName(FILE *fPtr, char * desc)
{
	Plane p = {0, 0, "", 0, {0}, {0}};

	rewind(fPtr);
	while(!feof(fPtr))
	{		
		p = readFromPlanesFile(fPtr);
		if( (equalString(desc, p.name) == 1) & 
		    (matchString(desc, p.name, len(desc)) == 1))
		{
			return 1;
		}
	}
	return 0;
}

int checkSeatNumber(Plane p, int num)
{
	int i;
	for(i = 0; i < 50; ++i)
	{
		if(num > 0)
		{
			if(p.seatNum[i] == num)
			{
				return 1;
			}			
		}
	}
	return 0;
}

int getTotalNumberOfReservedSeats(Plane p)
{
	int i, count;
	count = 0;
	for(i = 0; i < 50; ++i)
	{
		if(p.seatNum[i] > 0)
		{
			++count;
		}
	}
	return count;
}

int getTotalNumberOfFirstClassReservedSeats(Plane p)
{
	int i, count;
	count = 0;
	for(i = 0; i < p.seatsPerClass[0]; ++i)
	{
		if(p.seatNum[i] > 0)
		{
			++count;
		}
	}
	return count;
}

int getTotalNumberOfSecondClassReservedSeats(Plane p)
{
	int i, count;
	count = 0;
	for(i = 0; i < 50; ++i)
	{
		if(i > p.seatsPerClass[0])
		{
			if(p.seatNum[i] > 0)
			{
				++count;
			}
		}
	}
	return count;
}

int getSeatNumberClass(Plane p, int seatNum)
{
	if(seatNum > 0)
	{
		if(seatNum < getTotalNumberOfFirstClassSeats(p))
		{
			return 0;
		}
		else if( seatNum > getTotalNumberOfFirstClassSeats(p) &
				 seatNum < getTotalNumberOfSecondClassSeats(p))
		{
			return 1;
		}	
	}
	
	return -1;
}

int getTotalNumberOfSeats(Plane p)
{
	return p.seatsPerClass[0] + p.seatsPerClass[1];
}

int getTotalNumberOfFirstClassSeats(Plane p)
{
	return p.seatsPerClass[0];
}

int getTotalNumberOfSecondClassSeats(Plane p)
{
	return p.seatsPerClass[1];
}

int reserveSeat(Plane p, int seatNum)
{	
	int i, j;
	int seats = p.seatsPerClass[0] + p.seatsPerClass[1];
	for(i = 0; i < 50; i++)
	{
		if(seatNum > 0)
		{
			if(i < seats)
			{
				if(p.seatNum[i] == 0)
				{
					if(i == seatNum)
					{
						p.seatNum[i] = seatNum;
						return seatNum;
					}
					else if(i == (seats - 1))
					{
						for(j = (seats - 1); j > 0; --j)
						{
							if(p.seatNum[j] == 0)
							{
								p.seatNum[j] = j;
								return j;
							}
						}
					}					
				}				
			}
		}
	}
	return 0;
}

int unreserveSeat(Plane p, int seatNum)
{	
	int i;
	for(i = 0; i < 50; i++)
	{
		if(seatNum > 0)
		{			
			if(p.seatNum[i] == 0)
			{
				if(i == seatNum)
				{
					p.seatNum[i] = 0;
					return 1;
				}										
			}			
		}
	}
	return 0;
}


int promptPlaneInstructions()
{
	int option;
	printf("\n%s\n\n%s\n%s\n%s\n%s\n%s\n%s\n\n%s",
	"Please choose an option below to proceed.", 
	"1 - Create a text file for printing all records.",
	"2 - Create and save a new plane record.",
	"3 - Update an existing plane record.",
	"4 - Delete an existing plane record.", 
	"5 - View all plane records.", 
	"0 - Exit.",
	"Enter option: ");
	scanf("%d", &option);
	
	return option;	
}

Plane promptPlaneDetails()
{
	srand(850);
	int seats = (rand() % 10) * 100;
	seats = seats == 0 || seats >= 900 ? 850 : seats;
	Plane p = {0, 0, "", 0, {seats * 0.2, seats * 0.8}, {0}};
	
	printf("\nPlease enter plane details below.");
	printf("\nEnter Airport ID: ");
	fscanf( stdin, "%d", &p.airportId);		
	printf("\nEnter name: ");
	fscanf( stdin, "%s", p.name);	
			
	return p;
}

void printPlaneDetails(Plane p)
{
	printf("\n");
	printf("\nID: %d", p.planeId);
	printf("\nAirport ID: %d", p.airportId);
	printf("\nPlane Name: %s", p.name);
	printf("\nNumber of Miles traveled: %d", 
	p.milesTraveled);
	printf("\nTotal First Class Reserved seats: %d", 
	getTotalNumberOfFirstClassReservedSeats(p));
	printf("\nTotal Second Class Reserved seats: %d",
	getTotalNumberOfSecondClassReservedSeats(p));
	printf("\nTotal Reserved seats: %d", 
	getTotalNumberOfReservedSeats(p));
	printf("\nTotal First class seats: %d", 
	getTotalNumberOfFirstClassSeats(p));
	printf("\nTotal Second class seats: %d", 
	getTotalNumberOfSecondClassSeats(p));
	printf("\nTotal Number of seats: %d", 
	getTotalNumberOfSeats(p));
	printf("\n\n");
}

void printAllPlanes(FILE *fPtr)
{
	Plane p = {0, 0, "", 0, {0}, {0}};
	printf("\n\n\n%-6s%-14s%-8s%-9s%-10s%-10s%-6s\n", 
	"ID", "Airport ID", "Name", "Miles", "First", 
	"Second", "Seats");
	/* print underline */
	printf("%-6s%-14s%-8s%-9s%-10s%-10s%-6s\n",
	"--", "----------", "----", "-----", "-----",
	"------", "-----");
	rewind(fPtr);
	while(!feof(fPtr))
	{
		p = readFromPlanesFile(fPtr);
		if(p.planeId != 0)
		{
			printf("\n%-6d", p.planeId);
			printf("%-14d", p.airportId);
			printf("%-8s", p.name);
			printf("%-9d", p.milesTraveled);
			printf("%-10d", getTotalNumberOfFirstClassSeats(p));		
			printf("%-10d", getTotalNumberOfSecondClassSeats(p));			
			printf("%-6d", getTotalNumberOfSeats(p));
		}		
	}
	
	printf("\n\n\n\n");
}

void movePlanesCursor(FILE *fPtr, int pos)
{
	fseek(fPtr, ((pos - 1) * sizeof(Plane)), SEEK_SET);
}

Plane readFromPlanesFile(FILE *fPtr)
{
	Plane p = {0, 0, "", 0, {0}, {0}};
	fread(&p, sizeof(Plane), 1, fPtr);
	return p;
}

void writeToPlanesSeqFile(FILE *fPtr, Plane p)
{
	fprintf(fPtr, "%d, %d, %s, %d, %d, %d\n",
        p.planeId, p.airportId, p.name, p.milesTraveled, 
		p.seatsPerClass[0], p.seatsPerClass[1]); 
}

void writeToPlanesFile(FILE *fPtr, Plane p)
{
	fwrite(&p, sizeof(Plane), 1, fPtr);
	printf("\nWriting successful.\n");
}

void textPlanesFile(FILE *readPtr)
{
	FILE *writePtr;
	
	Plane p = {0, 0, "", 0, {0}, {0}};

	if( (writePtr = fopen("planes.txt", "w")) == NULL )
	{
		printf("File could not be opened.\n");
	}
	else
	{
		rewind(readPtr);
		while(!feof(readPtr))
		{
			p = readFromPlanesFile(readPtr);
			if(p.planeId != 0)
			{
				writeToPlanesSeqFile(writePtr, p);
			}
		}
		fclose(writePtr);
	}
}

void updatePlaneRecord(FILE *fPtr)
{
	int id;	
	Plane p = {0, 0, "", 0, {0}, {0}};
	
	printf("Enter ID to update: ");
	scanf("%d", &id);
	
	movePlanesCursor(fPtr, id);
	p = readFromPlanesFile(fPtr);
	if(p.planeId == 0 )
	{		
		printf("No plane record found with ID, \"%d\".", id);
	}
	else
	{
		printPlaneDetails(p);
		p = promptPlaneDetails();		
		removePlaneFromAirport(p.airportId, p.planeId);
		addPlaneToAirport(p.airportId, p.planeId);
		movePlanesCursor(fPtr, p.planeId);
		writeToPlanesFile(fPtr, p);		
		printf("\nUpdate successful.\n");		
	}
}

void deletePlaneRecord(FILE *fPtr)
{
	Plane p;
	Plane blankPlane = {0, 0, "", 0, {0}, {0}};
	int id;
	
	printf("Enter ID to delete: ");
	scanf("%d", &id);
	
	movePlanesCursor(fPtr, id);
	p = readFromPlanesFile(fPtr);
	if(p.planeId == 0)
	{
		printf("No customer record for ID, %d.", p.planeId);
	}
	else
	{
		movePlanesCursor(fPtr, p.planeId);
		writeToPlanesFile(fPtr, blankPlane);
		printf("\nDelete successful.\n");
	}
}

void newPlaneRecord(FILE *fPtr)
{
	Plane p = {0, 0, "", 0, {0}, {0}};
	int id;
	
	printf("Enter new ID: ");
	scanf("%d", &id);
		
	movePlanesCursor(fPtr, id);
	p = readFromPlanesFile(fPtr);
	
	if(p.planeId != 0)
	{		
		printf("Plane with ID \"%d\" contains information.\n", p.planeId);
	}	
	else
	{		
		p = promptPlaneDetails();	
		p.planeId = id;		
		addPlaneToAirport(p.airportId, p.planeId);
		movePlanesCursor(fPtr, p.planeId);
		writeToPlanesFile(fPtr, p);
		printf("\nInsertion successful.\n");		
	}	
}

int checkAirportId(int airportId)
{
	Airport port = {0, "", "", "", 0, 0, {0}};
	
	FILE *fPtr;
	if((fPtr = fopen("airports.dat", "rb")) == NULL)
	{		
		printf("File could not be opened.\n");		
	}
	else
	{
		moveAirportsCursor(fPtr, airportId);		
		port.id = airportId;
		port = readFromAirportsFile(fPtr);
		if(port.id != 0 & port.id == airportId)
		{
			return 1; // match found return as true
		}			
	} // end else
	fclose(fPtr);
	
	return 0;	
}

void addPlaneToAirport(int planeId, int airportId)
{
	Airport port = {0, "", "", "", 0, 0, {0}};
	int i, j;
	FILE *fPtr;
	if((fPtr = fopen("airports.dat", "rb+")) == NULL)
	{		
		printf("File could not be opened.\n");		
	}
	else
	{		
		if(checkAirportId(airportId) == 1)
		{			
			moveAirportsCursor(fPtr, airportId);
			port = readFromAirportsFile(fPtr);
			// check plane ID get index and then assign
			for(i = 0; i < 50; i++)
			{
				if(port.planes[i] == planeId) // plane exists
				{
					break;
				}
				else if(i == 49 & port.planes[i] != planeId) // add plane
				{
					for(j = 0; j > 0; --j)
					{
						if(port.planes[j] == 0)
						{
							port.planes[j] = planeId; // added
							break;
						}
					}
				}
			}
			writeToAirportsFile(fPtr, port);
		}		
	} // end else
	fclose(fPtr);		
}

void removePlaneFromAirport(int planeId, int airportId)
{
	Airport port = {0, "", "", "", 0, 0, {0}};
	int i;
	FILE *fPtr;
	if((fPtr = fopen("airports.dat", "rb+")) == NULL)
	{		
		printf("File could not be opened.\n");		
	}
	else
	{		
		if(checkAirportId(airportId) == 1)
		{			
			moveAirportsCursor(fPtr, airportId);
			// check plane ID get index and then assign
			for(i = 0; i < 50; i++)
			{
				if(port.planes[i] == planeId)
				{
					port.planes[i] = 0;
				}
			}
			writeToAirportsFile(fPtr, port);
		}		
	} // end else
	fclose(fPtr);
}



