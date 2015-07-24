#define MAX_PLANES 50

struct airport
{
	int id;
	char airportName[50]; 
	char country[30]; 
	char usualWeather[30]; 
	int timeFromBeirut; 
	int milesFromBeirut; 
	int planes[MAX_PLANES]; 
};
