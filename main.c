/***************************************************************************//**
  @file         main.c
  @author       Omar Bakier & Bahaa Sehsah
  @date         Wednesday,  27 April 2017
  @brief        (Banker algorithm)
*******************************************************************************/


//  Project 5 :
//  Detect deadlock and assign the system safety state using banker algorithm.
//
//  Created by Bahaa Mohamed Sehsah & Omar Abdulrazik Bakier.

#include <stdio.h>
#include<stdbool.h>

#define ProcessesNum    5       //number of processes.
#define ResourceNum     3       //number of resources.

/*
 * Global Initializations
 */
int total_inst[ResourceNum] = {10,5,7};                         //A,B,C resource instances.
int available[ResourceNum];                                     //array of available instances from each resource.
int need[ProcessesNum][ResourceNum];                            //the remaining instances from each resource for each process.
bool performed[ProcessesNum] = {false,false,false,false,false}; //array to distinguish between finished and unfinished processes.
/*  Assume these allocated instances for each process from each resource*/
int alloc[ProcessesNum][ResourceNum] = {0,1,0,
                                        2,0,0,
                                        3,0,2,
                                        2,1,1,
                                        0,0,2};

/*  Assume these remaining instances for each process from each resource*/
int max[ProcessesNum][ResourceNum]  =  {7,5,3,
                                        3,2,2,
                                        9,0,2,
                                        2,2,2,
                                        4,3,3};

/**
   @brief Calculate the remaining instances from each resource for each process.
 */
void Calculate_Need()
{
    for(int i=0 ; i<ProcessesNum ; i++)
    {
        for(int j=0 ; j<ResourceNum ; j++)
        {
            need[i][j] = max[i][j]-alloc[i][j];     //need(remaining instances to finish the job).
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
    if(performed[0] == false || performed[1] == false || performed[2] == false
       || performed[3] == false || performed[4] == false)
    {
        return false;
    }
    else    return true;
}

/**
   @brief The Banker algorithm.
 */
void Banker_Algorithim()
{
    /* Initializations */
    int  perform = -1;
    bool check_available;       //boolean to check instance of each resource.
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
                    perform = i;                 //store the number of process.
                    check_available = true;      //resource J has enough instances to be assigned (allocated).
                }
                else
                {
                    check_available = false;     //resource J doesn't have enough instances to be assigned (allocated).
                    break;                       //break to pick another unfinished process.
                }
            }

            /* Check if all resources have enough instances to be assigned to the process */
            if(check_available)
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
    printf("\n");
    /* Test detection of unsafe state by assigning total instances to be zero */
    for(int i=0 ; i<3 ; i++)    total_inst[i] = 0;
    for(int i=0 ; i<5 ; i++)    performed[i]  = false;
    Banker_Algorithim();                        // execute the banker algorithm again.
    printf("\n");
}