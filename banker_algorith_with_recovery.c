/***************************************************************************//**
  @file         main.c
  @author       Omar Bakier & Bahaa Sehsah
  @date         Wednesday,  27 April 2017
  @brief        (Deadlock Recovery - Resource Preemption)
*******************************************************************************/


//  Project 5 :
//  Eliminate deadlocks using resource preemption
//
//  Created by Bahaa Mohamed Sehsah & Omar Abdulrazik Bakier.

#include <stdio.h>
#include<stdbool.h>

#define ProcessesNum    5       //number of processes.
#define ResourceNum     3       //number of resources.

/*
 * Global Initializations
 */
int total_inst[ResourceNum] = {7,2,6};                          //A,B,C resource instances.
int available[ResourceNum];                                     //array of available instances from each resource.
bool check_available[3];                                        //array to check instance of each resource.
int need[ProcessesNum][ResourceNum];                            //the remaining instances from each resource for each process.
bool performed[ProcessesNum] = {false,false,false,false,false}; //array to distinguish between finished processes and not.

/*  Assume these allocated instances for each process from each resource*/
int alloc[ProcessesNum][ResourceNum] = {0,1,0,
                                        2,0,0,
                                        3,0,3,
                                        2,1,1,
                                        0,0,2};

/*  Assume these remaining instances for each process from each resource*/
int max[ProcessesNum][ResourceNum]  =  {0,1,0,
                                        4,0,2,
                                        3,0,4,
                                        3,1,1,
                                        0,0,4};

/**
   @brief Calculate the remaining instances from each resource for each process.
 */
void Calculate_Need()
{
    for(int i=0 ; i<ProcessesNum ; i++)
    {
        for(int j=0 ; j<ResourceNum ; j++)
        {
            need[i][j] = max[i][j]-alloc[i][j]; //need(remaining instances to finish the job).
        }
    }
}

/**
   @brief Calculate available instances from each resource after the process has finished its execution.
 */
void Calculate_Available()
{
    int sum;
    for(int j=0 ; j<ResourceNum ; j++)
    {
        sum = 0;
        for(int i=0 ; i<ProcessesNum ; i++)
        {
            sum += alloc[i][j];
        }
        available[j] = total_inst[j]-sum;
    }
}

/**
   @brief Indicates if any process is not done executing yet.
   @return returns True if done, returns False if not.
 */
bool Finished()
{
    if(performed[0] == false || performed[1] == false || performed[2] == false || performed[3] == false || performed[4] == false)
    {
        return false;
    }
    else    return true;
}

/**
   @brief Called to recover from deadlock after detecting it.
 */
void Recovery()
{
    int temp[3];                            //to store the instances required by the process that initiated the deadlock.

    /* pass through all processes(number of iterations)*/
    for(int i=0 ; i<ProcessesNum ; i++)
    {
        if(performed[i] == false)           //if any process is not done with execution yet (detecting the deadlock).
        {
            /* get the required instances for the unfinished process */
            for(int j=0 ; j<ResourceNum ; j++)
            {
                temp[j] = need[i][j];
            }

            /* pass through all processes again to detect the victim */
            for(int i=0 ; i<ProcessesNum ; i++)
            {
                /* compare all the needed instances for the deadlocked process with the allocated instances
                 * for all other processes, if the allocation is equal or larger than the needed/required,
                 * then pick that process to be the victim, and return the allocated instances it has
                 * to the available instances array */
                if(alloc[i][0] >= temp[0] && alloc[i][1] >= temp[1] && alloc[i][2] >= temp[2])
                {
                    /* get the allocated instances of the victim process */
                    for(int j=0 ; j<ResourceNum ; j++)
                    {
                        printf("P%d Is a victim.\n",i);
                        available[j] += alloc[i][j];    //add allocated instances of the victim to the available instances array.
                        alloc[i][j] = 0;                //set the allocated instances of the victim to zero.
                        Banker_Algorithim();            //call the banker_algorithm again to continue execution.
                        if(Finished)    return ;        //if the all processes are done, then exit the recovery.

                    }/* Got the allocated instances from the victim */

                }/* End of comparison */

            }/* End of detection */

        }/* End of checking all processes are done*/

    }

}/*End of function*/

/**
   @brief The Banker algorithm.
 */
void Banker_Algorithim()
{
    /* Initializations */
    int  perform = -1;
    Calculate_Available();      //calculate the available instances for each resource.
    Calculate_Need();           //calculate the needed instances for each process.

    /* Number of iterations */
    for(int i=0 ; i<ProcessesNum ; i++)
    {
        for(int i=0 ; i<ProcessesNum ; i++)
        {
            for(int j=0 ; j<ResourceNum ; j++)
            {
                /* check if available instances are larger than the needed instances for the current process,
                 * and if the current process is not finished */
                if(available[j] >= need[i][j] && performed[i] == false)
                {
                    perform = i;                    //store the number of process.
                    check_available[j] = true;      //resource J has enough instances to be assigned (allocated).
                }
                else
                {
                    check_available[j] = false;     //resource J doesn't have enough instances to be assigned (allocated).
                }
            }

            /* Check if all resources have enough instances to be assigned to the process */
            if(check_available[0]==true && check_available[1]==true && check_available[2]==true)
            {
                printf("P%d\t",perform);
                performed[perform] = true;          //set the current process as finished.
                for(int j=0 ; j<ResourceNum ; j++)
                {
                    //return the allocated instances for the finished process to the available instances.
                    available[j] += alloc[perform][j];
                    alloc[perform][j] = 0;          //set the allocated instances of the finished process to zero.
                }

            }

        }

    }


    if(Finished())
    {
        printf("\nSystem is in safe state.");   //if all processes finished executing.
    }
    else
    {
        printf("\nSystem is in unsafe state."); //if any process didn't finish executing.
    }
}


int main (int argc, char *argv[])
{
    Banker_Algorithim();                        // execute the banker algorithm.
    if(!Finished())                             // if there is a deadlock.
    {
        printf("\n\nRecovery takes place :\n");
        Recovery();                             // recover from deadlock.
    }
    printf("\n");
}
