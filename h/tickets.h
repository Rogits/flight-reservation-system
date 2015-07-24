int promptTicketInstructions();
int promptTicketUpdateInstructions();
Ticket promptTicketDetails();
void printTicketDetails(Ticket t);
void printAllTickets(FILE *fPtr);

void moveTicketsCursor(FILE *fPtr, int pos);
Ticket readFromTicketsFile(FILE *fPtr);
void writeToTicketsSeqFile(FILE *fPtr, Ticket t);
void writeToTicketsFile(FILE *fPtr, Ticket t);

void textTicketsFile(FILE *fPtr);
void newTicketRecord(FILE *fPtr);
void updateTicketRecord(FILE *fPtr);
void deleteTicketRecord(FILE *fPtr);

void processTickets();

void processTickets()
{
	FILE *fPtr;
	int choice = 0;
	
	if((fPtr = fopen("tickets.dat", "rb+")) == NULL)
	{
		if((fPtr = fopen("tickets.dat", "wb")) == NULL)
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
		while((choice = promptTicketInstructions()) != 0)
		{
			switch(choice)
			{
				case 1:
					textTicketsFile(fPtr);
					break;
				case 2:
					newTicketRecord(fPtr);
					break;
				case 3:
					updateTicketRecord(fPtr);
					break;
				case 4:
					deleteTicketRecord(fPtr);
					break;
				case 5:
					printAllTickets(fPtr);
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

int promptTicketInstructions()
{
	int option;
	printf("\n%s\n\n%s\n%s\n%s\n%s\n%s\n%s\n\n%s",
	"Please choose an option below to proceed.", 
	"1 - Create a text file for printing all records.",
	"2 - Create and save a new ticket record.",
	"3 - Update an existing ticket record.",
	"4 - Delete an existing ticket record.", 
	"5 - View all ticket records.", 
	"0 - Exit.",
	"Enter option: ");
	scanf("%d", &option);
	
	return option;	
}

Ticket promptTicketDetails()
{
	Ticket t = {0, 0, "", 0, "", 0, 0, "", 0, 0, 0, 0, 0, 0};
	Customer c = {0, "", "", "", 0,0,0, 0,0,0, ""};
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	Plane p = {0, 0, "", 0, {0}, {0}};
	Airport a = {0, "", "", "", 0, 0, {0}};
	FILE *readPtr, *writePtr; 
	
	printf("\n%s", "Enter customer ID: ");
	scanf("%d", &t.customerId);	
	c = getCustomerWithId(t.customerId);
	if(c.id == 0)
	{
		printf("\n\nCustomer ID %d does not exist.\n%s\n\n", 
		t.customerId,
		"So the system will try to create it.");
		// create a new customer record
		if((writePtr = fopen("customers.dat", "rb+")) == NULL)		
		{
			return t;
		}
		newCustomerRecord(writePtr);
		fclose(writePtr);
		
		printf("\nEnter customer ID again: ");
		scanf("%d", &t.customerId);
		c = getCustomerWithId(t.customerId);		
	}
	
	strcpy(t.names, getCustomerNames(c));	
	int customerMileage = getCustomerMileage(c);
	int customerNumOfFlights = getCustomerNumOfFlights(c);	
	
	printf("\nEnter flight number: ");
	scanf("%d", &t.flightNum);	
	f = getFlight(t.flightNum);
	if(f.flightNum == 0)
	{
		return t;
	}
	p = getPlane(f.planeId);
	a = getAirport(p.airportId);
	int flightDistance = a.milesFromBeirut;
	customerMileage += flightDistance;
	customerNumOfFlights += 1;
	updateCustomerMileage(c, customerMileage );	
	updateCustomerNumOfFlights(c, customerNumOfFlights);
		
	t.deptDay = getFlight(t.flightNum).deptDay;
	t.deptMon = getFlight(t.flightNum).deptMon;
	t.deptYr = getFlight(t.flightNum).deptYr;
	t.deptHour = getFlight(t.flightNum).deptTimeHour;
	t.deptMin = getFlight(t.flightNum).deptTimeMin;
		
	printf("\n%s", "Enter seat number: ");
	scanf("%d", &t.seatNum);
	int flightClass;
	if(checkSeatNumber(p, t.seatNum) == 1)
	{
		flightClass = getSeatNumberClass(p, t.seatNum);
		strcpy(t.ticketType,
			(flightClass == 0 ? "First Class" : "Second Class"));		
		t.costOfFlight = f.ticketPrice[flightClass];	
	}
	else
	{
		t.seatNum = reserveSeat(p, t.seatNum);
		flightClass = getSeatNumberClass(p, t.seatNum);
		strcpy(t.ticketType, 
			(flightClass == 0 ? "First Class" : "Second Class"));
		t.costOfFlight = f.ticketPrice[flightClass];
	}
		
	char * strinput;
	printf("\nIs ticket for discount(Yes/No): ");
	scanf("%s", strinput);	
	t.discount = matchString(strinput, "Yes", 3);	
	if(t.discount == 1 && checkOffersForCustomer(c) == 1)
	{
		printf("\nEnter reason for discount: ");
		scanf("\n");
		fgets(t.reasonForDiscount, sizeof(t.reasonForDiscount), stdin);
	}	
	
	return t;
}

void printTicketDetails(Ticket t)
{
	printf("\n\n%s%d\n", "Ticket ID: ", t.ticketNum);
	printf("\n%s%d\n", "Customer ID: ", t.customerId);
	printf("\n%s%s\n", "Customer names: ", t.names);
	printf("\n%s%d\n", "Flight number: ", t.flightNum);
	printf("\n%s%s\n", "Ticket type: ", t.ticketType);
	printf("\n%s%d\n", "Cost of flight: ", t.costOfFlight);
	printf("\n%s%s\n", "Ticket for discount: ", 
	(t.discount == 1 ? concat("Yes.\n\n", t.reasonForDiscount) : "No."));
	printf("\n%s%s\n", "Departure: ", 
	getFullDateAndTime(getDateAndTime(t.deptDay, t.deptMon, t.deptYr,
									  t.deptHour, t.deptMin, 0)) );
	printf("\n%s%d\n\n", "Seat number: ", t.seatNum);
}

void printAllTickets(FILE *fPtr)
{
	Ticket t = {0, 0, "", 0, "", 0, 0, "", 0, 0, 0, 0, 0, 0};
		
	printf("\n\n%-6s%-6s%-9s%-10s%-9s%-10s%-10s%-11s%s\n", 
	"No.", "ID", "Name", "Flight", "Class",
	"Cost($)", "Discount", "Departure", "Seat No." );
	printf("%-6s%-6s%-9s%-10s%-9s%-10s%-10s%-11s%s\n",
	"---", "--", "----", "------", "-----", 
	"-------", "--------", "---------", "--------");
	rewind(fPtr);
	while(!feof(fPtr))
	{
		t = readFromTicketsFile(fPtr);
		if(t.ticketNum != 0)
		{						
			printf("%-6d%-6d%-9s%-10d%-9s%-10d%-10s%d/%d/%-11d%d\n",
			t.ticketNum,
			t.customerId,
			t.names, 
			t.flightNum, extractTo(t.ticketType, ' '), 
			t.costOfFlight, (t.discount == 1 ? "Yes" : "No"),
			t.deptDay, t.deptMon, t.deptYr, t.seatNum);			
		}
	}	
}


void moveTicketsCursor(FILE *fPtr, int pos)
{
	fseek(fPtr, ((pos - 1) * sizeof(Ticket)), SEEK_SET);
}

Ticket readFromTicketsFile(FILE *fPtr)
{
	Ticket t = {0, 0, "", 0, "", 0, 0, "", 0, 0, 0, 0, 0, 0};
	fread(&t, sizeof(Ticket), 1, fPtr);
	return t;
}

void writeToTicketsSeqFile(FILE *fPtr, Ticket t)
{
	fprintf(fPtr, "%d, %d, %s, %d, %s, %d, %s, %d/%d/%d, %d",
	t.ticketNum, t.customerId, t.names, t.flightNum, 
	t.ticketType, t.costOfFlight, (t.discount == 1 ? "Yes": "No"),
	t.deptDay, t.deptMon, t.deptYr, t.seatNum);
}

void writeToTicketsFile(FILE *fPtr, Ticket t)
{
	fwrite(&t, sizeof(Ticket), 1, fPtr);
}

void textTicketsFile(FILE *readPtr)
{
	FILE *writePtr;
	
	Ticket t = {0, 0, "", 0, "", 0, 0, "", 0, 0, 0, 0, 0, 0};
	if((writePtr = fopen("planes.txt", "w")) == NULL )
	{
		printf("File could not be opened.\n");
	}
	else
	{
		rewind(readPtr);
		while(!feof(readPtr))
		{
			t = readFromTicketsFile(readPtr);
			if(t.ticketNum != 0)
			{
				writeToTicketsSeqFile(writePtr, t);
			}
		}
		fclose(writePtr);
	}
}


void updateTicketRecord(FILE *fPtr)
{
	int id;
	Ticket t = {0, 0, "", 0, "", 0, 0, "", 0, 0, 0, 0, 0, 0};
	char * opt;
	
	printf("Enter ticket number to update: ");
	scanf("%d", &id);
	
	moveTicketsCursor(fPtr, id);
	t = readFromTicketsFile(fPtr);
	if(t.ticketNum == 0 )
	{		
		printf("No ticket record found with ID, \"%d\".", id);
	}
	else
	{		
		printTicketDetails(t);
		int choice;
		if((choice = promptTicketUpdateInstructions()) !=0)
		{
			switch(choice)
			{
				case 1:
					printf("\nCurrently not supported.\n");
					break;
				case 2:					
					printf("\n\n");
					t = promptTicketDetails();		
					t.ticketNum = id;
					moveTicketsCursor(fPtr, id);
					writeToTicketsFile(fPtr, t);		
					printf("\nUpdate successful.\n");
					break;
				case 3:					
					printf("\nDo you wish to cancel your reservation(Yes/No): ");
					scanf("%s", opt);
					if(matchString(opt, "Yes", 3) == 1)
					{	
						deleteTicketRecord(fPtr);
						printf("\nReservation canceled.\n");
					}
					else
					{
						printf("\nReservation preserved.\n");
					}
					break;
				default:
					printf("\nInvalid option.\n");
					break;
			}		
		}		
	}
}

void deleteTicketRecord(FILE *fPtr)
{
	Ticket t;
	Ticket blankTicket = {0, 0, "", 0, "", 0, 0, "", 0, 0, 0, 0, 0, 0};
	Customer c = {0, "", "", "", 0,0,0, 0,0,0, ""};
	Flight f = {0, 0, 0, 0, 0, 0, 0, "", "", {0}, 0};
	Plane p = {0, 0, "", 0, {0}, {0}};
	Airport a = {0, "", "", "", 0, 0, {0}};
	int id;
	
	printf("Enter ticket number to delete: ");
	scanf("%d", &id);
	
	moveTicketsCursor(fPtr, id);
	t = readFromTicketsFile(fPtr);
	if(t.ticketNum == 0)
	{
		printf("No ticket record for ID, %d.", id);
	}
	else
	{		
		c = getCustomerWithId(t.customerId);
		f = getFlight(t.flightNum);
		p = getPlane(f.planeId);
		a = getAirport(p.airportId);
		int flightDistance, flightCount, newCustomerMileage, newCustomerFlightCount;
		flightDistance = a.milesFromBeirut;
		flightCount = getCustomerNumOfFlights(c);
		newCustomerMileage = (getCustomerMileage(c) - flightDistance);
		newCustomerFlightCount = flightCount - 1;
		updateCustomerMileage(c, newCustomerMileage);
		updateCustomerNumOfFlights(c, newCustomerFlightCount);
		if(unreserveSeat(p, t.seatNum) == 1)
		{
			moveTicketsCursor(fPtr, id);
			writeToTicketsFile(fPtr, blankTicket);
			printf("\nDelete successful.\n");
		}
		else
		{
			printf("\nFailed to cancel reservation.\n");
		}
	}
}

void newTicketRecord(FILE *fPtr)
{
	Ticket t = {0, 0, "", 0, "", 0, 0, "", 0, 0, 0, 0, 0, 0};
	int id;
	
	printf("Enter new ticket number: ");
	scanf("%d", &id);
	
	moveTicketsCursor(fPtr, id);
	t = readFromTicketsFile(fPtr);

	if(t.ticketNum != 0)
	{
		printf("Ticket record with ID \"%d\" contains information.\n", id);
	}	
	else
	{
		t = promptTicketDetails();	
		if(t.ticketNum != 0)
		{			
			t.ticketNum = id;
			moveTicketsCursor(fPtr, t.ticketNum);
			writeToTicketsFile(fPtr, t);
			printf("\nInsertion successful.\n");	
		}		
	}	
}


int promptTicketUpdateInstructions()
{
	int input;
	printf("\n\n%s\n%s\n%s\n%s\n%s\n\n%s", 
	"Please choose an option below.", 
	"1 - Change reservation from seat to seat.", 
	"2 - Change reservation from flight to flight.", 
	"3 - Cancel reservation.", 
	"0 - Exit.", 
	"Enter your option: ");
	scanf("%d", &input);
	
	return input;
}






	