struct flight 
{
	int flightNum; 
	int planeId; 
	int deptDay; 
	int deptMon; 
	int deptYr; 
	int deptTimeHour; 
	int deptTimeMin; 
	char deptDayName[20]; 
	char dest[50]; 
	int ticketPrice[2]; /* 0 - first class and 1 - second class */
	int milesToTravel; /* get from airport */
};
