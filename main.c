#include <stdio.h>
#include <string.h>
#include "h/util.h"
#include "h/date.h"
#include "h/customer.h"
#include "h/flight.h"
#include "h/plane.h"
#include "h/airport.h"
#include "h/ticket.h"
#include "h/offer.h"

typedef struct flight Flight;
typedef struct plane Plane;
typedef struct airport Airport;
typedef struct customer Customer;
typedef struct ticket Ticket;
typedef struct offer Offer;

#include "h/customers.h"
#include "h/airports.h"
#include "h/planes.h"
#include "h/flights.h"
#include "h/tickets.h"
#include "h/offers.h"

int promptMainInstructions();
void processMainInstructions();
void printArt();
void printHeader();

int main()
{
	processMainInstructions();
	return 0;
}

void processMainInstructions()
{
	int choice;
	printArt();
	while((choice = promptMainInstructions()) != 0)
	{
		switch(choice)
		{
			case 1:
				processCustomers();
				break;
			case 2: 
				processFlights();
				break;
			case 3:
				processTickets();
				break;
			case 4:
				processPlanes();
				break;
			case 5:
				processOffers();
				break;
			case 6:
				processAirports();
				break;
			default:
				printf("\nInvalid choice.\n");
				break;
		}
	}	
}

int promptMainInstructions()
{
	int option;
	printf("\n\n\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n%s",	
	"Please choose an option below to proceed.", 
	" 1 - Customers.",
	" 2 - Flights.",
	" 3 - Tickets.",
	" 4 - Planes.",
	" 5 - Offers.",
	" 6 - Airports.",
	" 0 - To Exit.", 
	"Enter your option: ");
	scanf("%d", &option);
	
	return option;	
}

void printArt()
{
	printf("\n\n\n\n\n\n\n\n");
	/* print your emblem here. */
	printHeader();
	printf("\n\n");
}

void printHeader()
{
	printf("\t\t%s",
		"Welcome to Beirut Flight Reservation System.");
}
