#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>

using namespace std;

#define MIN_PER_RANK 1 /* Nodes/Rank: How 'fat' the DAG should be.  */
#define MAX_PER_RANK 10
#define MIN_RANKS 5    /* Ranks: How 'tall' the DAG should be.  */
#define MAX_RANKS 10
#define PERCENT 30     /* Chance of having an Edge.  */

int main(int argc, char *argv[]){
    ofstream myfile ("example.txt");
    if (myfile.is_open()){
        int i, j, k,nodes = 0;
        srand (time (NULL));
        int ranks = MIN_RANKS
              + (rand () % (MAX_RANKS - MIN_RANKS + 1));

        for (i = 0; i < ranks; i++){
            /* New nodes of 'higher' rank than all nodes generated till now.  */
            int new_nodes = MIN_PER_RANK
                      + (rand () % (MAX_PER_RANK - MIN_PER_RANK + 1));
            /* Edges from old nodes ('nodes') to new ones ('new_nodes').  */
            for (j = 0; j < nodes; j++)
                for (k = 0; k < new_nodes; k++)
                    if ( (rand () % 100) < PERCENT)
                        myfile << j << " " << (k+nodes) <<"\n"; /* An Edge.  */

            nodes += new_nodes; /* Accumulate into old node set.  */
    }
        myfile.close();
  } else{
    cerr << "Unable to create a new file!\n";
    exit(1);   // call system to stop
  }
  return 0;
}