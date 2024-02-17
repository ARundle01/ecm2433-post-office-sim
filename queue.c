#include <queue.h>

/* Appends a node to the queue, given the data payload */
void appendNode(NODE ** head, NODE ** tail, int custId, int toleranceLim, 
                int serviceTime, int timeInterval)
{
    NODE *newNode; /* create a new node */
    NODE *lastNode = *tail; /* assign the tail to the previous node */

    if((newNode = (NODE *)malloc(sizeof(NODE)) ) == NULL) /* allocate memory to 
                                                             new node */
    {
        /* print an error message detailing error */
        printf("Memory Allocation Error when creating new Node\n");
        fprintf(stderr, "Error: %d: %s\n", errno, strerror(errno));
        exit(-1);
    }
    else
    {
        /* initialise node with given data and some default values */
        newNode->customerId = custId;
        newNode->toleranceLim = toleranceLim;
        newNode->serviceTime = serviceTime;
        newNode->timeWaiting = 0;
        newNode->timeServing = 0;
        newNode->joinedAt = timeInterval;

        newNode->next = NULL; /* set the next node in sequence to NULL */
    }

    if (*head == NULL) /* if head is NULL, it means the new node is the 
                          first to be added */
    {
        newNode->prev = NULL; /* set the previous node in list to NULL */
        *head = newNode; /* both the head and the tail of this list would 
                            be a single node */
        *tail = newNode;
    }
    else /* if head already exists, list is length >=1 nodes */
    {
        lastNode->next = newNode; /* set the next node after the tail 
                                     to the new node */
        newNode->prev = lastNode; /* set the prev node before the new
                                     node to tail */
        *tail = newNode; /* set the tail to the new node */
    }
}

/* Deletes a given node in the queue, given references to the 
 * head and tail. */
void deleteNode(NODE ** head, NODE ** tail, NODE * delNode)
{
    NODE *prevNode = delNode->prev; /* create pointers to the surrounding 
                                       nodes */
    NODE *nextNode = delNode->next;
    if (lengthOfQueue(head, tail) > 1)
    {
        if (*head == delNode) /* if we are deleting the head */
        {
            nextNode->prev = NULL; /* set prev node of next node to NULL */
            *head = nextNode; /* set head to next node of current node */
            delNode->next = NULL; /* set the next node of current node 
                                     to NULL */
        }
        else if (nextNode != NULL) /* if we are deleting other than 
                                      head or tail */
        {
            nextNode->prev = delNode->prev; /* set prev node of next node to 
                                               prev node of current node */
            prevNode->next = delNode->next; /* set next node of prev node to 
                                               next node of current node */
            delNode->prev = NULL; /* set prev and next of current node to NULL */
            delNode->next = NULL;
        }
        else if (*tail == delNode) /* if we are deleting the tail */
        {
            prevNode->next = NULL; /* set next node of prev node to NULL */
            *tail = prevNode; /* set tail to prev node of current node */
            delNode->prev = NULL; /* set prev node of current node to NULL */
        }
        free(delNode);
    }
    else /* if queue is empty */
    {
        *head = NULL; /* set head and tail to null, free the node */
        *tail = NULL;
        free(delNode);
    }
    
}


/* Prints the list starting from the given node */
void printList(NODE * node)
{
    NODE *lastNode; /* initialise last node in print */
    while (node != NULL) /* whilst we have not reached a null node */
    {
        /* print the data payload of each node */
        printf("Customer: %d\nTolerance: %d\nService Time: %d\nJoined at %d\n", 
               node->customerId, node->toleranceLim, node->serviceTime, 
               node->joinedAt);
        lastNode = node; /* set last node to current node */
        node = node->next; /* set current node to next node */
    }
}


/* Returns the length of the queue */
int lengthOfQueue(NODE ** head, NODE ** tail)
{
    /* create pointers to current node and last node in search */
    NODE *lastNode;
    NODE *currentNode = *head;
    int len = 0;
    while (currentNode != NULL) /* if the current node is not NULL */
    {
        len++; /* increment len */
        lastNode = currentNode; /* move forward in list */
        currentNode = currentNode->next;
    }
    return len; /* return length once at end of list */
}


/* increments the timeWaiting of a given node */
void incrementWaiting(NODE * node)
{
    (node->timeWaiting)++;
}


/* increments the timeWaiting of all nodes in the queue */
void incrementAllWaiting(NODE ** head)
{
    NODE *currentNode = *head;
    NODE *lastNode;
    
    while (currentNode != NULL) /* Iterate over queue, as seen before */
    {
        incrementWaiting(currentNode); /* increment the current node */

        lastNode = currentNode;
        currentNode = currentNode->next; /* move forwared in list */
    }
}

/* predicate which returns TRUE if a node is bored (the time between them 
 * joining the queue has exceeded their tolerance */
BOOLEAN isBored(NODE * node, int timeInterval)
{
    int joinedAt = node->joinedAt;
    int tolerance = node->toleranceLim;
    int timeSince = timeInterval - joinedAt;
    if (tolerance <= timeSince) /* if the time since they joined is larger than
                                   their tolerance, they are now boread */
        return TRUE;
    else
        return FALSE;
}


/* Removes all bored nodes in the list */
void removeAllBored(NODE ** head, NODE ** tail, int * numTimedOut, 
                    int timeInterval)
{
    NODE *currentNode = *head;
    NODE *lastNode;
 
    while (currentNode != NULL) /* iterate over list */
    {
        if (isBored(currentNode, timeInterval) == TRUE) /* if they are bored */
        {
            (*numTimedOut)++; /* increment the number of timed-out people */
            deleteNode(head, tail, currentNode); /* delete the node */
            currentNode = *head; /* start the search from the top, as the 
                                    current node was deleted */
        }
        else
        {
            lastNode = currentNode; /* move forward in the list */
            currentNode = currentNode->next;
        }
    }
}


/* adds a new customer with randomized variables using the given means */
void addNewCustomerNode(NODE ** head, NODE ** tail, gsl_rng ** pointerToRngGen,
                        int * numUnfulfilled, int customerId, 
                        float meanTolerance, float meanServingTime, 
                        int maxQueueLength, int timeInterval)
{
    int tolerance;
    int servingTime;

    /* generate a random tolerance using the Poisson distribution */
    tolerance = gsl_ran_poisson(*pointerToRngGen, meanTolerance);
    if (tolerance < 1) /* if the tolerance is zero, set it to 1 */
        tolerance = 1;
    /* generate a random servingTime using the Poisson distribution */
    servingTime = gsl_ran_poisson(*pointerToRngGen, meanServingTime);
    if (servingTime < 1) /* if it is zero, set to 1 */
        servingTime = 1;
    
    /* if the queue is full and it is not an unlimited queue */
    if (lengthOfQueue(head, tail) == maxQueueLength && maxQueueLength != -1)
    {
        (*numUnfulfilled)++; /* increment the number of unfulfilled people */
    }
    else
    {
        /* otherwise append a new node with the random variables */
        appendNode(head, tail, customerId, tolerance, servingTime, timeInterval);
    }
}




