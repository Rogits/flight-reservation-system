Customer promptCustomerDetails();
void printCustomerDetails(Customer client);
void moveCustomersCursor(FILE *fPtr, int pos);
Customer readFromCustomersFile(FILE *fPtr);
void writeToCustomersFile(FILE *fPtr, Customer client);
void writeToCustomersSeqFile(FILE *fPtr, Customer client);

int promptCustomerInstructions();
void printAllCustomers(FILE *fPtr);

void textCustomersFile(FILE *fPtr);
void newCustomerRecord(FILE *fPtr);
void updateCustomerRecord(FILE *fPtr);
void deleteCustomerRecord(FILE *fPtr);

Customer getCustomerWithId(int customerId);

char * getCustomerNames(Customer c);
int getCustomerMileage(Customer c);
int getCustomerNumOfFlights(Customer c);
void updateCustomerMileage(Customer c, int miles);
void updateCustomerNumOfFlights(Customer c, int flights);

void processCustomers();

void processCustomers()
{
	FILE *fPtr;
	int choice = 0;
	
	if((fPtr = fopen("customers.dat", "rb+")) == NULL)
	{
		if((fPtr = fopen("customers.dat", "wb")) == NULL)
		{
			printf("\nFile could not be opened.\n\n");
		}
		else
		{
			printf("\nNew file created.\n");
		}
	}
	else
	{
		while((choice = promptCustomerInstructions()) != 0)
		{
			switch(choice)
			{
				case 1:
					textCustomersFile(fPtr);
					break;
				case 2:
					newCustomerRecord(fPtr);
					break;
				case 3:
					updateCustomerRecord(fPtr);
					break;
				case 4:
					deleteCustomerRecord(fPtr);
					break;
				case 5:
					printAllCustomers(fPtr);
					break;
				default:
					printf("Incorrect choice.\n");
					break;				
			}
		}
		
		fclose(fPtr);
		if(choice == 0)
		{
			printf("\nExiting...\n");
		}
	}
}

Customer promptCustomerDetails()
{
	Customer client = {0, "", "", "", 0,0,0, 0,0,0, ""};
	printf("\nPlease enter customer details below.");
	printf("\nEnter First name: ");
	fscanf( stdin, "%s", client.fname);
	printf("\nEnter Last name: ");
	fscanf(stdin, "%s", client.lname);
	printf("\nEnter Day of Birth: ");
	fscanf( stdin, "%d", &client.d);
	printf("\nEnter Month of Birth: ");
	fscanf( stdin, "%d", &client.m);
	printf("\nEnter Year of Birth: ");
	fscanf( stdin, "%d", &client.y);
	printf("\nEnter Country: ");
	fscanf( stdin, "%s", client.country);
	printf("\nEnter Phone: ");
	fscanf( stdin, "%d", &client.phone);
	printf("\nEnter Country Code(+XXX): ");
	fscanf( stdin, "%s", &client.ccode);
	
	return client;
}

void printCustomerDetails(Customer client)
{
	printf("\nID: %d", client.id);
	printf("\nFirst Name: %s", client.fname);
	printf("\nLast Name: %s", client.lname);
	printf("\nDate of Birth: %s", 
	getFormattedDate(getDate(client.d, client.m, client.y), "%a, %B %Y")
	/* %d/%d/%d - client.d, client.m, client.y*/);
	printf("\nCountry: %s", client.country);
	printf("\nPhone: %s %d", client.ccode, client.phone);
	printf("\nFlights: %d", client.flights);
	printf("\nMiles: %d", client.miles);
	printf("\n");
}

void moveCustomersCursor(FILE *fPtr, int pos)
{
	fseek(fPtr, ((pos - 1) * sizeof(Customer)), SEEK_SET);
}

Customer readFromCustomersFile(FILE *fPtr)
{
	Customer client = {0, "", "", "", 0,0,0, 0,0,0, ""};
	fread(&client, sizeof(Customer), 1, fPtr);
	return client;
}

void writeToCustomersFile(FILE *fPtr, Customer client)
{
	fwrite(&client, sizeof(Customer), 1, fPtr);
	printf("\nWriting successful.\n");
}

void writeToCustomersSeqFile(FILE *fPtr, Customer client)
{
	fprintf(fPtr, "%d, %s %s, %d/%d/%d, %s, %s %d, %d, %d\n",
        client.id, client.fname, client.lname, 
		client.d, client.m, client.y, client.country, 
		client.ccode, client.phone, client.flights, client.miles);	
}

int promptCustomerInstructions()
{
	int option;
	printf("\n\n%s\n\n%s\n%s\n%s\n%s\n%s\n%s\n\n%s",
	"Please choose an option below to proceed.", 
	"1 - Create a text file for printing all records.",
	"2 - Create and save a new customer record.",
	"3 - Update an existing customer record.",
	"4 - Delete an existing customer record.", 
	"5 - View all customer records.", 
	"0 - Exit.",
	"Enter option: ");
	scanf("%d", &option);
	
	return option;	
}

void printAllCustomers(FILE *fPtr)
{
	Customer client = {0, "", "", "", 0,0,0, 0,0,0, ""};
		
	printf("\n%-6s%-9s%-12s%-10s%-7s%19s%10s\n", 
	"ID", "Names", "DOB", "Country", "Phone",	
	"Flights", "Miles" );
	/* print underline */
	printf("%-6s%-9s%-12s%-10s%-7s%19s%10s\n",
	"--", "-----", "---", "-------", "-----", "-------", "-----");
	rewind(fPtr);
	while(!feof(fPtr))
	{
		client = readFromCustomersFile(fPtr);
		if(client.id != 0)
		{			
			printf("\n%-6d%s %-5s%2d/%2d/%-6d%-10s%s %-14d%-12d%d",
			client.id, trim(client.fname, 3),
			concat(trim(client.lname, 1), "."),
			client.d, client.m, client.y, client.country,
			client.ccode, client.phone, client.flights, client.miles
			);
		}		
	}
	
	printf("\n\n");
}


void textCustomersFile(FILE *readPtr)
{
	FILE *writePtr;
	
	Customer client = {0, "", "", "", 0,0,0, 0,0,0, ""};
	
	if( (writePtr = fopen("customers.txt", "w")) == NULL )
	{
		printf("File could not be opened.\n");
	}
	else
	{
		rewind(readPtr);
		while(!feof(readPtr))
		{
			client = readFromCustomersFile(readPtr);
			if(client.id != 0)
			{
				writeToCustomersSeqFile(writePtr, client);
			}
		}
		fclose(writePtr);
	}
}

void updateCustomerRecord(FILE *fPtr)
{
	int id;
	Customer client = {0, "", "", "", 0,0,0, 0,0,0, ""};
	
	printf("Enter ID to update: ");
	scanf("%d", &id);
	
	moveCustomersCursor(fPtr, id);
	client = readFromCustomersFile(fPtr);
	if(client.id == 0)
	{
		printf("No customer record found with ID, \"%d\".", id);
	}
	else
	{
		printCustomerDetails(client);
		client = promptCustomerDetails();
		client.id = id;
		moveCustomersCursor(fPtr, id);
		writeToCustomersFile(fPtr, client);		
		printf("\nUpdate successful.\n");
	}
}

void deleteCustomerRecord(FILE *fPtr)
{
	Customer client;
	Customer blankClient = {0, "", "", "", 0,0,0, 0,0,0, ""};
	int id;
	
	printf("Enter ID to delete: ");
	scanf("%d", &id);
	
	moveCustomersCursor(fPtr, id);
	client = readFromCustomersFile(fPtr);
	if(client.id == 0)
	{
		printf("No customer record for ID, %d.", id);
	}
	else
	{
		moveCustomersCursor(fPtr, id);
		writeToCustomersFile(fPtr, blankClient);
		printf("\nDelete successful.\n");
	}
}

void newCustomerRecord(FILE *fPtr)
{
	Customer client = {0, "", "", "", 0,0,0, 0,0,0, ""};
	int id;
	
	printf("Enter new ID: ");
	scanf("%d", &id);
	
	moveCustomersCursor(fPtr, id);
	client = readFromCustomersFile(fPtr);
	
	if(client.id != 0)
	{
		printf("Customer with ID \"%d\" contains information.\n", client.id);
	}
	else
	{
		client = promptCustomerDetails();
		client.id = id;
		moveCustomersCursor(fPtr, id);
		writeToCustomersFile(fPtr, client);
		printf("\nInsertion successful.\n");
	}	
}

Customer getCustomerWithId(int customerId)
{
	Customer client = {0, "", "", "", 0,0,0, 0,0,0, ""};
	
	FILE *fPtr;
	if((fPtr = fopen("customers.dat", "rb")) == NULL)
	{
		printf("\nFile could not be opened.\n");		
	}
	else
	{	
		if(customerId != 0)
		{
			moveCustomersCursor(fPtr, customerId);
			client = readFromCustomersFile(fPtr);	
		}
	}	
	fclose(fPtr);
	return client;	
}

char * getCustomerNames(Customer c)
{
	if(c.id > 0)
	{
		return concat(c.fname, c.lname);
	}
	return "";
}

int getCustomerMileage(Customer c)
{
	if(c.id > 0)
	{
		return c.miles;
	}
	return c.miles;
}

int getCustomerNumOfFlights(Customer c)
{	
	if(c.id > 0)
	{
		return c.flights;
	}
	return c.flights;
}

void updateCustomerMileage(Customer c, int miles)
{	
	FILE *fPtr;
	if((fPtr = fopen("customers.dat", "rb+")) == NULL)
	{
		printf("File could not be opened.\n");		
	}
	else
	{	
		c.miles = miles;
		moveCustomersCursor(fPtr, c.id);
		writeToCustomersFile(fPtr, c);	
		fclose(fPtr);
	}	
}

void updateCustomerNumOfFlights(Customer c, int flights)
{
	FILE *fPtr;
	if((fPtr = fopen("customers.dat", "rb+")) == NULL)
	{
		printf("File could not be opened.\n");		
	}
	else
	{	
		c.flights = flights;
		moveCustomersCursor(fPtr, c.id);
		writeToCustomersFile(fPtr, c);	
		fclose(fPtr);		
	}
}



