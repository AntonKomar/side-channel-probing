#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "probe.h"
#include "libnumberpic.h"

int main(int argc, char *argv[]) {
    

    int measure_c = 500;
    int measurements[measure_c][4];

    for (int i = 0; i < measure_c; i++) {
        measurements[i][0] = reload((char *) numbers[1]);
        measurements[i][1] = reload((char *) numbers[5]);
        measurements[i][2] = reload((char *) numbers[7]);
        measurements[i][3] = reload((char *) &placenum);

        flush((char *) numbers[1]);
        flush((char *) numbers[5]);
        flush((char *) numbers[7]);
        flush((char *) &placenum);

        usleep(1000);
    }

    // write to file
    FILE *fp;

    fp = fopen("measurements.csv", "w");
    for (int i = 0; i < measure_c; i++) {
        fprintf(fp, "%i,%i,%i,%i\n", measurements[i][0], measurements[i][1], measurements[i][2], measurements[i][3]);
    }
    fclose(fp);

    return 0;
}
