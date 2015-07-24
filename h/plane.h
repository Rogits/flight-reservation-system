#define SIZE 1000

struct plane
{
	int planeId;
	int airportId; 
	char name[20];
	int milesTraveled;
	int seatsPerClass[2];
	int seatNum[SIZE];
};
