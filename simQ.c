/*------------------------------ Includes -----------------------------------*/
#include <simQ.h>

/*------------------------------ Main Function ------------------------------*/
int main (int argc, char **argv)
{ 
    if(argc != 4) /* If there are more or less than 4 args, print error msg and
                     exit */
    {
        printf("Incorrect number of arguments supplied, please try again\n");
        fprintf(stderr, "Error: 22: %s\n", strerror(22));
        exit(1);
    }
    else /* Otherwise, retrieve arguments from argv and init the simulation */
    {
        int inputLen = strlen(argv[1]); /* get the string length of input
                                           and output file names */
        int outputLen = strlen(argv[3]);

        /* define variables to store input */
        char input[inputLen];
        char output[outputLen];

        int simNum;

        /* calculate the length of name prepending .txt */
        char inNum[inputLen - 3];
        char outNum[outputLen - 3];
        
        simNum = atoi(argv[2]); /* retrieve number of simulations */

        strcpy(input, argv[1]); /* retrieve name of input file */
        
        if(sscanf(input, "%s.txt", inNum) != 1) /* check it ends in .txt */
        {
            printf("Given input file is not a .txt file\n");
            fprintf(stderr, "Error: Input file is not a text (.txt) file\n");
            fflush(stdout);
            fflush(stderr);
            exit(-1);
        }

        strcpy(output, argv[3]); /* retrieve name of output file */

        if(sscanf(output, "%s.txt", outNum) != 1) /* check it ends in .txt */
        {
            printf("Given output file is not a .txt file");
            fprintf(stderr, "Error: output file is not a text (.txt) file");
            fflush(stdout);
            fflush(stderr);
            exit(-1);
        }

        startSim(input, output, simNum); /* Initialise the simulation */
    }
    return 0;
}

/*------------------------------ Functions ----------------------------------*/

/* Function initialises the simulation, by checking input and interpreting
 * the user input arguments.
 * This will also call the runSim() function. */
void startSim(char inputFile[], char outputFile[], int simNum)
{
    printf("No. Sims: %d\nInput File: %s\nOutput File: %s\n", 
           simNum, inputFile, outputFile);

    clearFile(outputFile); /* write over the file of given name, effectively 
                              clearing it */
    
    FILE *fptr = NULL;
    /* attempt to open output file with append and read acces */
    if ((fptr = fopen(outputFile, "a+")) == NULL)
    {
        printf("Unable to open file for read and append access\n");
        fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
        exit(1);
    }
    
    /* append the command line inputs to the output file */
    fprintf(fptr, "Number of Simulations: %d\n", simNum);
    fprintf(fptr, "Input File: %s\n", inputFile);
    fprintf(fptr, "Output File: %s\n", outputFile);

    /* initialise the queue */
    NODE *nodeHead = NULL; /* initialise head to NULL */
    NODE *nodeTail = NULL; /* initialise tail to NULL */
    /* initialise the list of serve points */
    SERVE *serveHead = NULL; /* initialise head to NULL */
    SERVE *serveTail = NULL; /* initialise tail to NULL */
    
    /* define variables to retrieve from input file */
    int maxQueueLength;
    int numServicePoints;
    int closingTime;
    float meanTolerance;
    float meanServingTime;
    float meanSpawnRate;
   
    /* retrieve values from input file, place into above variables */
    getVariablesFromFile(inputFile, &maxQueueLength, &numServicePoints, 
                         &closingTime, &meanTolerance, &meanServingTime, 
                         &meanSpawnRate);
   
    /* write the retrieved values into the output file */ 
    fprintf(fptr, "maxQueueLength: %d\n", maxQueueLength);
    fprintf(fptr, "numServicePoints: %d\n", numServicePoints);
    fprintf(fptr, "closingTime: %d\n", closingTime);
    fprintf(fptr, "meanTolerance: %f\n", meanTolerance);
    fprintf(fptr, "meanServingTime: %f\n", meanServingTime);
    fprintf(fptr, "meanSpawnRate: %f\n", meanSpawnRate);
   
    /* initialise counters for multiple simulations */
    int totalWait = 0;
    int timeToFinish = 0;
    int numFulfill = 0;
    int numUnfulfill = 0;
    int numTimedOut = 0;
    
    
    if (simNum > 1) /* if running multiple sims */
    {
        /* declare counters and totals */
        int currentSimNum = 0;
        int avgWaitForSim;
        int totalOfAvgWait;
        /* loop simulation simNum times */
        while (currentSimNum < simNum)
        {
            /* run simulation */
            runSim(meanTolerance, meanServingTime, meanSpawnRate, simNum, 
                   closingTime, &nodeHead, &nodeTail, &serveHead, &serveTail, 
                   maxQueueLength, numServicePoints, outputFile, &fptr, 
                   &totalWait, &timeToFinish, &numFulfill, &numUnfulfill, 
                   &numTimedOut);

            /* iterate to counter of current sim */
            currentSimNum++;
            avgWaitForSim = totalWait/numFulfill; /* calculate average wait */
            totalOfAvgWait += avgWaitForSim; /* add to total average wait */
        }
        /* if the number of fulfilled people is zero, average is also zero */
        int avgFulfill;
        if (numFulfill == 0)
            avgFulfill = 0;
        else /* otherwise, calculate the average */
            avgFulfill = numFulfill/simNum;
        
        /* if the number of unfulfilled people is zero, average is zero */
        int avgUnfulfill;
        if (numUnfulfill == 0)
            avgUnfulfill = 0;
        else /* otherwise, calculate average */
            avgUnfulfill = numUnfulfill/simNum;

        /* repeat above for timed-out people */
        int avgTimedOut;
        if (numTimedOut == 0)
            avgTimedOut = 0;
        else
            avgTimedOut = numTimedOut/simNum;

        /* if the total time to finish is zero, average is zero */
        int avgTimeToFinish;
        if (timeToFinish == 0)
            avgTimeToFinish = 0;
        else /* otherwise, calculate average */
            avgTimeToFinish = timeToFinish/simNum;

        /* average wait per simulation is the total of all averages,
         * divided by the number of simulations */
        int averageWait;
        averageWait = totalOfAvgWait/simNum;
        /* write the statistics to the output file */
        fprintf(fptr, "Average number of Fulfilled across %d simulations: %d\n",
                      simNum, avgFulfill);

        fprintf(fptr, "Average number of Unfulfilled across %d simulations: %d"
                      "\n", simNum, avgUnfulfill);

        fprintf(fptr, "Average number of Timed-Out across %d simulations: %d\n",
                      simNum, avgTimedOut);

        fprintf(fptr, "Average time from Closing to Finishing, over %d " 
                      "simulations: %d\n", simNum, avgTimeToFinish);

        fprintf(fptr, "Average waiting time: %d\n", averageWait);

    }
    else if (simNum == 1) /* if running only one simulation */
    {
        /* run sim once */
        runSim(meanTolerance, meanServingTime, meanSpawnRate, simNum, closingTime,
               &nodeHead, &nodeTail, &serveHead, &serveTail, maxQueueLength, 
               numServicePoints, outputFile, &fptr, &totalWait, &timeToFinish, 
               &numFulfill, &numUnfulfill, &numTimedOut);
        
        /* calculate only average and write statistics to output file */
        int averageWait = totalWait/numFulfill;        
        fprintf(fptr, "Time from Closing to Finishing: %d\n", timeToFinish);
        fprintf(fptr, "Total number of people fulfilled: %d\n", numFulfill);
        fprintf(fptr, "Total number of people unfulfilled: %d\n", numUnfulfill);
        fprintf(fptr, "Total number of people timed-out: %d\n", numTimedOut);
        fprintf(fptr, "Total time spent waiting: %d\n", totalWait);
        fprintf(fptr, "Average waiting time: %d\n", averageWait);
    }
    else /* if simNum is less than 1, there is an error */
    {
        printf("Number of Simulations less than 1\n");
        fprintf(stderr, "Error: Must have at least 1 simulation\n");
    }

    fclose(fptr); /* close the output file */
    
    /* free the head and tail of queue */
    if (nodeHead == nodeTail) /* if the head and tail are the same */
        free(nodeHead); /* free only the head (as it is also the tail) */
    else /* otherwise, free both head and tail */
    {
        free(nodeHead);
        free(nodeTail);
    }

    /* do the same as above, but for the serve list */
    if (serveHead == serveTail)
        free(serveHead);
    else
    {
        free(serveHead);
        free(serveTail);
    }
}


/* runs the simulation a single time, updating various statistics via pointer
 * reference */
void runSim(float meanTolerance, float meanServingTime, float meanSpawnRate, 
            int simNum, int closingTime, NODE ** nodeHead, NODE ** nodeTail, 
            SERVE ** serveHead, SERVE ** serveTail, int maxQueueLength, 
            int numServicePoints, char outputFile[], FILE ** fptr, 
            int * totalWait, int * timeToFinish, int * totalFulfill, 
            int * totalUnfulfill, int * totalTimedOut)
{
    /* declare sim specific counters */
    int numFulfill = 0;
    int numUnfulfill = 0;
    int numTimedOut = 0;
    int timeWaited = 0;
    int spoint;
    
    /* declare starting time interval */
    int timeInterval = 0;
    /* declare starting customer ID */
    int customerId = 0;
    /* declare flag for end of loop */
    BOOLEAN isDone = FALSE;
    
    /* initialise the gsl random number generator */
    const gsl_rng_type *T;
    gsl_rng *rngGen;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    rngGen = gsl_rng_alloc(T);
    gsl_rng_set(rngGen, time(NULL));
    
    /* add the specified number of service points, from the input file */
    addNServes(serveHead, serveTail, numServicePoints);

    while(isDone != TRUE) /* while the simulation is not done */
    {
        /* Fulfill customers */
        fulfillServes(serveHead, &numFulfill, &timeWaited);

        /* Serve the first customer in the queue */
        updateFirstEmpty(serveHead, nodeHead, nodeTail);

        /* Remove any timed-out customers */
        removeAllBored(nodeHead, nodeTail, &numTimedOut, timeInterval);

        /* Spawn more customers, as long as we are not closed */
        if (timeInterval < closingTime)
        {
            /* number of people to spawn using Poisson distribution */
            int numToSpawn = gsl_ran_poisson(rngGen, meanSpawnRate);
            int spawn = 0;
            /* while there are still more left to spawn */
            while (spawn != numToSpawn)
            {
                /* attempt to add a node to the queue */
                addNewCustomerNode(nodeHead, nodeTail, &rngGen, &numUnfulfill, 
                                   customerId++, meanTolerance, meanServingTime,
                                   maxQueueLength, timeInterval);
                spawn++; /* increment number of people currently spawned */
            }
        }

        if (simNum == 1) /* if desired number of sims is 1 */
        {
           if (timeInterval == 0) /* write marker for opening time */
               fprintf(*fptr, "---------- Opening Time! ----------\n");
           if (timeInterval == closingTime) /* write marker for closing time */
               fprintf(*fptr, "---------- Closing Time! ----------\n");

           /* write marker for start of a time interval */
           fprintf(*fptr, "----- Interval %d -----\n", timeInterval);

           /* write number of people currently being served */
           int numServing = lengthOfServe(serveHead) - hasEmptyServe(serveHead);
           fprintf(*fptr, "Number of People being served: %d\n", numServing);

           /* write number of people waiting in queue */
           fprintf(*fptr, "Number of People in Queue: %d\n", 
                          lengthOfQueue(nodeHead, nodeTail));

           /* write the total number of people fulfilled */
           fprintf(*fptr, "Number of Fulfilled people: %d\n", numFulfill);

           /* write the total number of people unfulfilled */
           fprintf(*fptr, "Number of Unfulfilled people: %d\n", numUnfulfill);

           /* write the total number of people timed-out */
           fprintf(*fptr, "Number of Timed-Out people: %d\n", numTimedOut);
        }
        
        /* Check if queue is empty, all are served and is past closing time */
        if (lengthOfQueue(nodeHead, nodeTail) == 0       &&
            hasEmptyServe(serveHead) == numServicePoints &&
            timeInterval >= closingTime)
        {
            if (simNum == 1) /* if only doing one sim */
                /* write that simulation has finished */
                fprintf(*fptr, "---------- Simulation Done ----------\n");
           
            /* update values of wanted statistics */ 
            *timeToFinish += (timeInterval - closingTime);
            *totalWait += timeWaited;
            *totalFulfill += numFulfill;
            *totalUnfulfill += numUnfulfill;
            *totalTimedOut += numTimedOut;
            /* set completion flag to TRUE, ending loop */
            isDone = TRUE;
        }
        else /* if simulation is not complete */
        {
            timeInterval++; /* increment to next time interval */
            incrementAllServes(serveHead); /* increment all being served */
            incrementAllWaiting(nodeHead); /* increment all waiting */
        }
    }
    
    gsl_rng_free(rngGen); /* free memory used by gsl rng machine */
    deleteAllServes(serveHead, serveTail); /* delete and free all serves */
}

