#include <fileManip.h>

/* Opens the given output file to write into it.
 * This creates the file if it does not exist or
 * overwrites it as an empty file if it does */
void clearFile(char outputFile[51])
{
    FILE *fptr = NULL;
    if ((fptr = fopen(outputFile, "w")) == NULL) /* open as write only */
    {
        printf("Unable to open file for write access.\n");
        fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
        exit(1);
    }
    else
    {
        fclose(fptr); /* close file */
    }

}


/* Opens the given output file and appends the given string to it. */
void writeToFile(char outputFile[51], char string[])
{
    FILE *fptr = NULL;
    if((fptr = fopen(outputFile, "a+")) == NULL) /* If file cannot be opened, 
                                                    print error msg and exit */
    {
        printf("Unable to open file for read and append access.\n");
        fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
        exit(1);
    }
    else /* otherwise, append given string to file */
    {
        fprintf(fptr, "%s", string);
        fclose(fptr); /* close file */
    }
}


/* retrieves the variables from the given inputFile and places them into the 
 * pointers supplied to the function. The input file must be given in the
 * correct format, otherwise the function will report an error */
void getVariablesFromFile(char inputFile[], int *maxQueueLength, 
                          int *numServicePoints, int *closingTime, 
                          float *meanTolerance, float *meanServingTime, 
                          float *meanSpawnRate)
{
    FILE *fptr = NULL; 
    if ((fptr = fopen(inputFile, "r")) == NULL) /* attempt to open the file 
                                                   read-only */
    {
        printf("Unable to open file for read access.\n");
        fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
        exit(1);
    }
    else
    {   
        int varCount;
        varCount = fscanf(fptr, "maxQueueLength %d\nnumServicePoints %d\n"
                                "closingTime %d\nmeanTolerance %f\n"
                                "meanServingTime %f\nmeanSpawnRate %f", 
                                maxQueueLength, numServicePoints, 
                                closingTime, meanTolerance, 
                                meanServingTime, meanSpawnRate);
                                /* scan the file in the given format and 
                                 * assign to given variables */
        if (varCount != 6) /* if there are not 6 variables assigned */
        {
            /* there must be an error in the formatting of the file */
            printf("File is not formatted correctly.\n");
            fprintf(stderr, "Error: fscanf successfully matched and assigned %d"
                            " inputs, with 6 expected.\n", varCount);
            exit(-1);
        }
        else
        {
            fclose(fptr);
        }
    }
}

