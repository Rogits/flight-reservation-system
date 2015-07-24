int promptFlightInstructions();
Flight promptFlightDetails();
void printFlightDetails(Flight f);
void printAllFlights(FILE *fPtr);

void moveFlightsCursor(FILE *fPtr, int pos);
Flight readFromFlightsFile(FILE *fPtr);
void writeToFlightsSeqFile(FILE *fPtr, Flight f);
void writeToFlightsFile(FILE *fPtr, Flight f);

void textFlightsFile(FILE *fPtr);
void newFlightRecord(FILE *fPtr);
void updateFlightRecord(FILE *fPtr);
void deleteFlightRecord(FILE *fPtr);

int checkPlaneFromAirport(int planeId);
Airport getAirport(int airportId);
Plane getPlane(int planeId);

int getPlaneId(int flightNum);

Flight getFlight(int flightNum);

void processFlights();

void processFlights()
{
	FILE *fPtr;
	int choice = 0;
	
	if((fPtr = fopen("flights.dat", "rb+")) == NULL)
	{
		if((fPtr = fopen("flights.dat", "wb")) == NULL)
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
		while((choice = promptFlightInstructions()) != 0)
		{
			switch(choice)
			{
				case 1:
					textFlightsFile(fPtr);
					break;
				case 2:
					newFlightRecord(fPtr);
					break;
				case 3:
					updateFlightRecord(fPtr);
					break;
				case 4:
					deleteFlightRecord(fPtr);
					break;
				case 5:
					printAllFlights(fPtr);
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

Flight promptFlightDetails()
{
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	Airport a = {0, "", "", "", 0, 0, {0}};
	printf("\nPlease enter flight details below.\n");
	
	printf("\nEnter plane ID: ");
	fscanf(stdin, "%d", &f.planeId);
	
	if(checkPlaneFromAirport(f.planeId) == 1)
	{
		printf("\nEnter departure date(Day): ");
		fscanf( stdin, "%d", &f.deptDay);
		printf("\nEnter departure date(Month): ");
		fscanf( stdin, "%d", &f.deptMon);
		printf("\nEnter departure date(Year): ");
		fscanf( stdin, "%d", &f.deptYr);
		printf("\nEnter departure time(Hour): ");
		fscanf( stdin, "%d", &f.deptTimeHour);
		printf("\nEnter departure time(Minute): ");
		fscanf( stdin, "%d", &f.deptTimeMin);		
		printf("\nEnter cost of flight(First Class): ");	
		fscanf( stdin, "%d", &f.ticketPrice[0]);
		printf("\nEnter cost of flight(Second Class): ");
		fscanf(stdin, "%d", &f.ticketPrice[1]);		
		strcpy(f.deptDayName, getDay(getDate(f.deptDay, f.deptMon, f.deptYr)));
		a = getAirport(f.planeId);
		strcpy(f.dest, a.airportName);
		f.milesToTravel = a.milesFromBeirut;		
	}
	else
	{
		printf("\nPlane ID %d does not exist.\n", f.planeId);		
	}
	
	return f;
}

void printFlightDetails(Flight f)
{
	printf("\nFlight Number: %d", f.flightNum);
	printf("\nPlane ID: %d", f.planeId);
	printf("\nDeparture Date: %d/%d/%d", f.deptDay, f.deptMon, f.deptYr);
	printf("\nDeparture Time: %d:%d", f.deptTimeHour, f.deptTimeMin);
	printf("\nDeparture Day: %s", f.deptDayName);
	printf("\nDeparture Month: %s", getMonthName(getDate(0,f.deptMon,0)) );
	printf("\nDeparture Full Date and Time: %s", 
	getFullDateAndTime(
	getDateAndTime(f.deptDay, f.deptMon, f.deptYr, f.deptTimeHour, f.deptTimeMin, 0)));
	printf("\nDestination: %s", f.dest);
	printf("\nFirst Class Cost: $%d", f.ticketPrice[0]);
	printf("\nSecond Class Cost: $%d", f.ticketPrice[1]);
	printf("\nDistance(miles): %d", f.milesToTravel);
	printf("\n");
}

void printAllFlights(FILE *fPtr)
{
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	printf("\n%-9s%-8s%-13s%-7s%-14s%-8s%-9s%s\n", 
	"Flight", "Plane", "Dept. Date", "Time", "Destination",
	"First", "Second", "Distance");
	/* print underline */
	printf("%-9s%-8s%-13s%-7s%-14s%-8s%-9s%s\n",
	"------", "-----", "----------", "----", "-----------", 
	"-----", "------", "--------");
	rewind(fPtr);
	while(!feof(fPtr))
	{
		f = readFromFlightsFile(fPtr);
		if(f.flightNum != 0)
		{						
			printf("\n%-9d%-8d%d/%d/%-7d%d:%-4d%-14s$%-7d$%-9d%d\n",
			f.flightNum, f.planeId, f.deptDay, f.deptMon, f.deptYr,
			f.deptTimeHour, f.deptTimeMin, trim(f.dest, 11), 
			f.ticketPrice[0], f.ticketPrice[1], 
			f.milesToTravel);			
		}		
	}
	
	printf("\n\n");
}

void moveFlightsCursor(FILE *fPtr, int pos)
{
	fseek(fPtr, ((pos - 1) * sizeof(Flight)), SEEK_SET);
}

Flight readFromFlightsFile(FILE *fPtr)
{
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	fread(&f, sizeof(Flight), 1, fPtr);
	return f;
}

void writeToFlightsSeqFile(FILE *fPtr, Flight f)
{
	fprintf(fPtr, "%d, %d, %s, %d:%d, %d/%d/%d, %s, %d, %d, %d\n",
        f.flightNum, f.planeId, f.deptDayName, f.deptTimeHour, 
		f.deptTimeMin, f.deptDay, f.deptMon, f.deptYr,  f.dest, 
		f.ticketPrice[0], f.ticketPrice[1], f.milesToTravel); 
}

void writeToFlightsFile(FILE *fPtr, Flight f)
{
	fwrite(&f, sizeof(Flight), 1, fPtr);
}

void textFlightsFile(FILE *readPtr)
{
	FILE *writePtr;
	
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	
	if( (writePtr = fopen("flights.txt", "w")) == NULL )
	{
		printf("File could not be opened.\n");
	}
	else
	{
		rewind(readPtr);
		while(!feof(readPtr))
		{
			f = readFromFlightsFile(readPtr);
			if(f.flightNum != 0)
			{
				writeToFlightsSeqFile(writePtr, f);
			}
		}
		fclose(writePtr);
	}
}

void updateFlightRecord(FILE *fPtr)
{
	int num;
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	
	printf("Enter Flight number to update: ");
	scanf("%d", &num);
	
	moveFlightsCursor(fPtr, num);
	f = readFromFlightsFile(fPtr);
	if(f.flightNum == 0)
	{
		printf("No flight record found with flight number, \"%d\".", num);
	}
	else
	{
		printFlightDetails(f);
		f = promptFlightDetails();		
		moveFlightsCursor(fPtr, num);
		writeToFlightsFile(fPtr, f);		
		printf("\nUpdate successful.\n");
	}
}

void deleteFlightRecord(FILE *fPtr)
{
	Flight f;
	Flight emptyFlight = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	int num;
	
	printf("Enter Flight number to delete: ");
	scanf("%d", &num);
	
	moveFlightsCursor(fPtr, num);
	f = readFromFlightsFile(fPtr);
	if(f.flightNum == 0)
	{
		printf("No flight record with flight number, %d.", num);
	}
	else
	{
		moveFlightsCursor(fPtr, num);
		writeToFlightsFile(fPtr, emptyFlight);
		printf("\nDelete successful.\n");
	}
}

void newFlightRecord(FILE *fPtr)
{
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	int id;
	
	printf("Enter Flight Number: ");
	scanf("%d", &id);
	
	moveFlightsCursor(fPtr, id);
	f = readFromFlightsFile(fPtr);
	
	if(f.flightNum != 0)
	{
		printf("Flight with ID \"%d\" contains information.\n", f.flightNum);
	}
	else
	{
		f.flightNum = id;
		f = promptFlightDetails();		
		moveFlightsCursor(fPtr, id);
		writeToFlightsFile(fPtr, f);
		printf("\nInsertion successful.\n");
	}	
}

int checkPlaneFromAirport(int planeId)
{
	Plane p = {0, 0, "", 0, {0}, {0}};
	Airport port = {0, "", "", "", 0, 0, {0}};
	
	FILE *fPtr;
	if((fPtr = fopen("planes.dat", "rb")) == NULL)
	{		
		printf("File could not be opened.\n");		
	}
	else
	{
		movePlanesCursor(fPtr, planeId);
		p = readFromPlanesFile(fPtr);
		if(p.planeId != 0)
		{			
			port = getAirport(p.airportId); 
			if(port.id != 0)
			{
				int i;
				for(i=0; i < 50; ++i)
				{
					if(port.planes[i] == planeId)
					{
						return 1; // match found
					}
				}
			}
		}		
	} // end else
	fclose(fPtr);
	
	return 0;
}

Airport getAirport(int airportId)
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
		port = readFromAirportsFile(fPtr);		
		if(port.id != 0)
		{
			return port; // match found return as true
		}		
	} // end else
	fclose(fPtr);
	
	return port;	
}

Plane getPlane(int planeId)
{
	Plane p = {0, 0, "", 0, {0}, {0}};
	FILE *fPtr;
	if(checkPlaneFromAirport(planeId) == 1)
	{
		if((fPtr = fopen("planes.dat", "rb")) == NULL)
		{
			printf("\nFile could not be opened.\n");
		}
		else
		{
			movePlanesCursor(fPtr, planeId);
			p = readFromPlanesFile(fPtr);
			if(p.planeId != 0)
			{
				return p;
			}
		}
	}
	return p;
}

int getPlaneId(int flightNum)
{
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	FILE *fPtr;
	if((fPtr = fopen("flights.dat", "rb")) == NULL)
	{
		return 0;
	}
	else
	{
		moveFlightsCursor(fPtr, flightNum);
		f = readFromFlightsFile(fPtr);
		if(f.flightNum > 0)
		{
			return f.planeId;
		}
	}
	return 0;
}

Flight getFlight(int flightNum)
{
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	FILE *fPtr;
	if((fPtr = fopen("flights.dat", "rb")) == NULL)
	{
		return f;
	}
	else
	{
		moveFlightsCursor(fPtr, flightNum);
		f = readFromFlightsFile(fPtr);
		if(f.flightNum > 0)
		{
			return f;
		}
	}
	return f;	
	
}


int promptFlightInstructions()
{
	int option;
	printf("\n%s\n\n%s\n%s\n%s\n%s\n%s\n%s\n\n%s",
	"Please choose an option below to proceed.", 
	"1 - Create a text file for printing all records.",
	"2 - Create and save a new flight record.",
	"3 - Update an existing flight record.",
	"4 - Delete an existing flight record.", 
	"5 - View all flight records.", 
	"0 - Exit.",
	"Enter option: ");
	scanf("%d", &option);
	
	return option;	
}



