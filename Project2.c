#include <stdio.h>
#include <math.h>

// Maximum number of data points: 5040 for all data, 56 for 1 day, 1680 for 1 month
#define MAX_READINGS 5040

// Calculates the hourly average for each day
void calculateHourlyAverage(float readings[MAX_READINGS], float monthData[30][16], int offset) {
    float hourlySum;
    int startIndex, hourBlock, day, hour;

    // Iterate over each day and calculate the average for every hour block
    for (day = 0; day < 30; day++) {
        for (hour = 0; hour < 14; hour++) {
            startIndex = (56 * day + offset) + 4 * hour; // Starting index of 4-hour block
            hourlySum = 0;

            // Sum the readings for the 4-hour block
            for (hourBlock = 0; hourBlock < 4; hourBlock++) {
                hourlySum += readings[startIndex + hourBlock];
            }

            // Store the hourly average
            monthData[day][hour] = hourlySum / 4;
        }
    }
}

// Calculates the daily mean and standard deviation
void calculateDailyMeanAndSD(float readings[MAX_READINGS], float dayData[30][16], int offset) {
    float dailySum;
    int startIndex, readingIndex, day;

    // Calculate the daily mean
    for (day = 0; day < 30; day++) {
        startIndex = (56 * day) + offset; // Starting index for the day
        dailySum = 0;

        // Sum all readings for the day
        for (readingIndex = 0; readingIndex < 56; readingIndex++) {
            dailySum += readings[startIndex + readingIndex];
        }

        // Store the mean in column 14
        dayData[day][14] = dailySum / 56;
    }

    // Calculate the daily standard deviation
    for (day = 0; day < 30; day++) {
        startIndex = (56 * day) + offset;
        dailySum = 0;

        // Calculate the squared differences from the mean
        for (readingIndex = 0; readingIndex < 56; readingIndex++) {
            dailySum += pow((readings[startIndex + readingIndex] - dayData[day][14]), 2);
        }

        // Store the standard deviation in column 15
        dayData[day][15] = sqrt(dailySum / 56);
    }
}

// Calculate the monthly average for a specific hour
float calculateMonthlyAverage(float monthData[30][16], int hourIndex) {
    float totalSum = 0, average;
    int day;

    // Sum the averages for the specified hour across all days
    for (day = 0; day < 30; day++) {
        totalSum += monthData[day][hourIndex];
    }

    // Calculate and return the monthly average
    average = totalSum / 30;
    return average;
}

// Display table headers for the output
void displayTableHeaders() {
    int i;
    float hour;

    printf("\n\n");
    printf("\n");
    for (i = 1; i < 11; i++) {
        printf("~~~~~");
    }
    printf(" Time (N Hour) ");
    for (i = 1; i < 11; i++) {
        printf("~~~~~");
    }
    printf("\n");

    // Display AM and PM time slots
    for (hour = 7; hour < 12; hour++) {
        printf("%.2f AM|", hour);
    }
    printf("12.00 PM|");

    for (hour = 1; hour < 9; hour++) {
        printf("%.2f PM|", hour);
    }
    printf("\n");

    // Display hour blocks (0-13)
    for (i = 1; i < 24; i++) {
        printf("-----");
    }
    printf("\n");
    for (i = 0; i < 3; i++) {
        printf("   %d   |", i);
    }
    for (i = 3; i < 6; i++) {
        printf("    %d   |", i);
    }
    for (i = 6; i < 10; i++) {
        printf("   %d   |", i);
    }
    for (i = 10; i < 14; i++) {
        printf("  %d   |", i);
    }

    // Display calendar header
    printf("\n\n~~ Calendar (N Month) ~~");
    printf("\n");
    printf("%6s |%5s  |%6s |\n", "April", "May", "June");
    for (i = 1; i < 5; i++) {
        printf("------");
    }
    printf("\n");
    for (i = 0; i < 3; i++) {
        printf("   %d   |", i);
    }
}

// Print the monthly data
void printMonthlyData(float monthData[30][16]) {
    int day, hour, x;
    float timeSlot;

    printf("\n");
    for (x = 1; x < 14; x++) {
        printf("~~~");
    }
    printf(" Average by Hour ");
    for (x = 1; x < 13; x++) {
        printf("~~~");
    }
    printf("\n");

    // Display hour headers
    printf("          ");
    for (timeSlot = 7; timeSlot < 12; timeSlot++) {
    printf(" %.0fAM |", timeSlot);
    }

    printf("12PM |");

    for (timeSlot = 1; timeSlot < 9; timeSlot++) {
    printf(" %.0fPM |", timeSlot);
    }

    printf("Mean |");
    printf(" SD |");
    printf("\n");
    for (x = 1; x < 22; x++) {
        printf("-----");
    }

    // Print data for each day
    for (day = 0; day < 30; day++) {
        printf("\nDay %-2d:  |", day + 1);
        for (hour = 0; hour < 16; hour++) {
            printf("%.2f|", monthData[day][hour]);
        }
    }
}

// Get user input for the hour and month
void getUserInput(int *hourIndex, int *monthIndex) {
    printf("\n\nCalculate monthly average of? (Enter -1 to end program)\n");
    printf("\nEnter N Hour : ");
    scanf("%d", hourIndex); // Read input into the address pointed by hourIndex
    printf("Enter N Month  : ");
    scanf("%d", monthIndex); // Read input into the address pointed by monthIndex
}

int main() {
    FILE *file = fopen("proj2.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    float readings[MAX_READINGS];
    float aprilData[30][16];
    float mayData[30][16];
    float juneData[30][16];

    int readingIndex = 0, hourIndex, monthIndex;
    float averageResult;

    // Read data from the file into the readings array
    while (fscanf(file, "%f", &readings[readingIndex]) != EOF && readingIndex < MAX_READINGS) {
        readingIndex++;
    }
    fclose(file);

    // Process the data for each month
    calculateHourlyAverage(readings, aprilData, 0);
    calculateHourlyAverage(readings, mayData, 1680);
    calculateHourlyAverage(readings, juneData, 3360);

    calculateDailyMeanAndSD(readings, aprilData, 0);
    calculateDailyMeanAndSD(readings, mayData, 1680);
    calculateDailyMeanAndSD(readings, juneData, 3360);

    // Print the results for each month
    printf("APRIL :");
    printMonthlyData(aprilData);
    printf("\n\nMAY :");
    printMonthlyData(mayData);
    printf("\n\nJUNE :");
    printMonthlyData(juneData);

    displayTableHeaders();
    getUserInput(&hourIndex, &monthIndex);

    // Validate user input
    while (((hourIndex < 0) || (hourIndex > 13)) || ((monthIndex < 0) || (monthIndex > 2))) {
        if (hourIndex == -1 || monthIndex == -1) {
            return 0; // Exit if user enters -1
        }
        printf("\nInvalid input N\n");
        getUserInput(&hourIndex, &monthIndex);
    }

    // Calculate and display the monthly average
    if (monthIndex == 0) {
        averageResult = calculateMonthlyAverage(aprilData, hourIndex);
    } else if (monthIndex == 1) {
        averageResult = calculateMonthlyAverage(mayData, hourIndex);
    } else if (monthIndex == 2) {
        averageResult = calculateMonthlyAverage(juneData, hourIndex);
    }

    printf("\nMonthly average : %.2f\n", averageResult);

    return 0;
}

