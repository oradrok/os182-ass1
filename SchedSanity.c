#include "types.h"
#include "user.h"

#define NUM_OF_CHILDS 12
#define CALC_MEDIUM_SIZE_LOOP 1000000
#define CALC_LARGE_SIZE_LOOP 200000000
#define IO_MEDIUM_SIZE_LOOP 1000
#define IO_LARGE_SIZE_LOOP 20000


int gcd ( int a, int b ){
    int c;
    while ( a != 0 ) {
       c = a; a = b%a;  b = c;
    }
    return b;
}

void medium_loop_calculation(){
	for(int j=0;j<CALC_MEDIUM_SIZE_LOOP;++j)
		gcd(j+1,j+2);
}

void large_loop_calculation(){
	for(int j=0;j<CALC_LARGE_SIZE_LOOP;++j)
		gcd(j+1,j+2);
}

void medium_loop_print(){
	for(int j=0;j<IO_MEDIUM_SIZE_LOOP;++j){
		printf(1, "print something\n");
	}
}

void large_loop_print(){
	for(int j=0;j<IO_LARGE_SIZE_LOOP;++j){
		printf(1, "print something else\n");
	}
}


int main(int argc, char *argv[])
{
int cfsd = 0;

#ifdef CFSD
	cfsd = 1;
#endif

	int pids[NUM_OF_CHILDS];

	for(int i=0; i< NUM_OF_CHILDS; i++){
		int pid;
	    pid = fork();
		if(pid == 0){
			if(cfsd) set_priority(i%3 + 1);

			if(i%4 == 0)
				medium_loop_calculation();
			if(i%4 == 1)
				large_loop_calculation();
			if(i%4 == 2)
				medium_loop_print();
			if(i%4 == 3)
				large_loop_print();

			exit();
		}
		else
			pids[i] = pid;

	}


	int sum_wtime[4];
	int sum_rtime[4];
	int sum_iotime[4];
	int wtime;
	int rtime;
	int iotime;

	for(int i=0; i< NUM_OF_CHILDS; i++){
			wait2(pids[i],&wtime,&rtime,&iotime);
			sum_wtime[i%4] += wtime;
			sum_rtime[i%4] += rtime;
			sum_iotime[i%4] += iotime;

	}


	printf(1,"Calculation Medium -  Wait time: %d,  Run time: %d, IO Time: %d\n\n", sum_wtime[0]/NUM_OF_CHILDS, sum_rtime[0]/NUM_OF_CHILDS, sum_iotime[0]/NUM_OF_CHILDS);
	printf(1,"Calculation Large -  Wait time: %d,  Run time: %d, IO Time: %d\n\n", sum_wtime[1]/NUM_OF_CHILDS, sum_rtime[1]/NUM_OF_CHILDS, sum_iotime[1]/NUM_OF_CHILDS);
	printf(1,"Calculation + IO Medium -  Wait time: %d,  Run time: %d, IO Time: %d\n\n", sum_wtime[2]/NUM_OF_CHILDS, sum_rtime[2]/NUM_OF_CHILDS, sum_iotime[2]/NUM_OF_CHILDS);
	printf(1,"Calculation + IO Large -  Wait time: %d,  Run time: %d, IO Time: %d\n\n", sum_wtime[3]/NUM_OF_CHILDS, sum_rtime[3]/NUM_OF_CHILDS, sum_iotime[3]/NUM_OF_CHILDS);

	exit();
}