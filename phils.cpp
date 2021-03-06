//Compile with: mpic++ philosophers.cpp -o philosophers
//run with: 	mpiexec -np 6 philosophers


#include "mpi.h"
#include "pomerize.h"
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//Defining run limits
#define MAXMESSAGES 10000

//Defining request types
#define CHOP_REQ 1	//request
#define CHOP_RES 2	//response
#define CHOP_REL 3	//release
#define PHIL_DONE 4 //philosopher is done

const string fileBase = "outFile";

void phil(int this_id, int p_in)
{
	int id = this_id;
	int p = p_in - 1;
	int sigIn;
	int sigOut;
	int msgSent = 0;
	MPI_Status st;

	srand(id + time(NULL));

	//filesetup
	pomerize P; //Poem generator object

	int leftNeighbor = id - 1;
  	int rightNeighbor = id % p;

  	string lFile = fileBase + to_string(leftNeighbor);
  	string rFile = fileBase + to_string(rightNeighbor);
  	ofstream foutLeft(lFile.c_str(), ios::out | ios::app );
  	ofstream foutRight(rFile.c_str(), ios::out | ios::app );

	printf("Philosopher %d spawned\n", id);
	while(msgSent < MAXMESSAGES)
	{
		//thinking
		printf("Philosopher %d is sleeping\n", id);
		sleep(rand()%2);
		printf("Philosopher %d waiting\n", id);
		MPI_Send(&sigOut, 1, MPI_INT, 0, CHOP_REQ, MPI_COMM_WORLD); //Ask for chopsticks
		MPI_Recv(&sigIn, 1, MPI_INT, 0, CHOP_RES, MPI_COMM_WORLD, &st); //Wait for chopsticks
		printf("Philosopher %d is writing %d \n", id, msgSent);
		msgSent++;
		//Do the thing

		//Poem code
		string stanza1, stanza2, stanza3;
		stanza1 = P.getLine();
		foutLeft << stanza1 << endl;
		foutRight << stanza1 << endl;

		stanza2 = P.getLine();
		foutLeft << stanza2 << endl;
		foutRight << stanza2 << endl;

		stanza3 = P.getLine();
		foutLeft << stanza3 << endl << endl;
		foutRight << stanza3 << endl << endl;
		
		//sleep(rand()%2);

		MPI_Send(&sigOut, 1, MPI_INT, 0, CHOP_REL, MPI_COMM_WORLD); //Release chopsticks
		printf("Philosopher %d is done \n", id);
	}
	printf("Philosopher %d is exiting\n", id);
	MPI_Send(&sigOut, 1, MPI_INT, 0, PHIL_DONE, MPI_COMM_WORLD); //Get the check from the waiter

	//Close files
	foutLeft.close();
	foutRight.close();
}

void waiter(int this_id, int p_in)
{	
	int id = this_id;
	int p = p_in - 1;
	int source;
	int done = 0;
	int sigIn;
	int sigOut;
	MPI_Status st;	
	printf("Table %d spawned\n", id);
	
	std::list<int> q;

	//generate chops list and set to availible
	bool chops[p];
	for(int i = 0; i < p; i++)
	{
		chops[i] = true;
	}

	srand(id + time(NULL));

	
	while(done != p)
	{
		printf("Scanning... %d/%d\n", done, p);
		//get signals
		MPI_Recv(&sigIn, 1, MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		source = st.MPI_SOURCE;

		//request
		if(st.MPI_TAG == CHOP_REQ)
		{
			//check forks
			if(chops[source % p] && chops[source - 1])
			{	
				//reserve forks
				chops[source % p] = false;
				chops[source - 1] = false;

				//signal phil
				MPI_Send(&sigOut, 1, MPI_INT, source, CHOP_RES, MPI_COMM_WORLD);
			}
			else
			{
				//No forks available
				q.push_back(source);
			}
		}

		//release
		if(st.MPI_TAG == CHOP_REL)
		{
			//set forks free
			chops[source % p] = true;
			chops[source - 1] = true;

			//scan queue if there's waiting
			if(!q.empty())
			{
				//scan list of qaiting
				for(std::list<int>::iterator i = q.begin();
					i != q.end();
					i++)
				{	
					//we can just reuse the source even
					//though it's not the source an more
					source = *i;

					if(chops[source % p] && chops[source - 1])
					{	
						//reserve forks
						chops[source % p] = false;
						chops[source - 1] = false;

						//signal phil
						MPI_Send(&sigOut, 1, MPI_INT, source, CHOP_RES, MPI_COMM_WORLD);
						//drop from queue
						i = q.erase(i);
					}

				}
			}
		}

		//finished
		if(st.MPI_TAG == PHIL_DONE)
		{	
			//register that the phil has sent die sig
			done++;
		}
	}

}

int main(int argc, char** argv)
{
	int id; //this processes rank
	int p; 	//number of processes

	//mpi setup
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (p < 3) {
	    MPI::Finalize ( );
	    std::cerr << "ERR: Need at least 2 philosophers and one waiter!" << std::endl;
	    return 1; //non-normal exit
  }
	//First thread is waiter, others are phils
	if(!id)
	{
		waiter(id,p);	//spawn waiter
	} 
	else 
	{
		phil(id,p); //spawn philosopher
	}

	//We're done!
	MPI_Finalize();
	return 0;
}
