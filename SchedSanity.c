#include "types.h"
#include "user.h"
#define NUM_OF_CHILDS 11
#define CLAC_MEDIUM_SIZE_LOOP 1000000
#define CALC_LARGE_SIZE_LOOP 20000000
#define IO_MEDIUM_SIZE_LOOP 1000
#define IO_LARGE_SIZE_LOOP 20000

int
gcd ( int a, int b )
{
  int c;
  while ( a != 0 ) {
     c = a; a = b%a;  b = c;
  }
  return b;
}



void small_loop_calculation(){
	for(int j=0;j<CLAC_MEDIUM_SIZE_LOOP;++j)
		gcd(j+1,j+2);

}



void large_loop_calculation(){
	for(int j=0;j<CALC_LARGE_SIZE_LOOP;++j)
		gcd(j+1,j+2);

}


void small_loop_print(){
	for(int j=0;j<IO_MEDIUM_SIZE_LOOP;++j){
		printf(1, "hello world!\n");
	}
}

void large_loop_print(){
	for(int j=0;j<IO_LARGE_SIZE_LOOP;++j){
		printf(1, "hello world!\n");
	}
}



int main(int argc, char *argv[])
{


/*#ifdef CFSD
	int cfsd = 1;
#else
	int cfsd = 0;
#endif*/
//SchedSanity

	int pids[NUM_OF_CHILDS];

	for(int i=0; i< NUM_OF_CHILDS; i++){
		int pid;
	    pid = fork();
		if(pid == 0){
//			if(cfsd) set_priority(i%3 + 1);
			if(i%4 == 0)
				small_loop_calculation(); //Calculation only - These processes will perform asimple calculation within a medium sized loop
			if(i%4 == 1)
				large_loop_calculation(); //Calculation only – These processes will perform simple calculation within a very large loop
			if(i%4 == 2)
				small_loop_print();// Calculation + IO – These processes will perform printing to screen within a medium sized loop
			if(i%4 == 3)
				large_loop_print(); // Calculation + IO – These processes will perform printing to screen within a very large loop

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


	printf(1,"Calculation Medium -  Wait time: %d,  Run time: %d, IO Time: %d\n\n",sum_wtime[0]/NUM_OF_CHILDS,sum_rtime[0]/NUM_OF_CHILDS,sum_iotime[0]/NUM_OF_CHILDS);
	printf(1,"Calculation Large -  Wait time: %d,  Run time: %d, IO Time: %d\n\n",sum_wtime[1]/NUM_OF_CHILDS,sum_rtime[1]/NUM_OF_CHILDS,sum_iotime[1]/NUM_OF_CHILDS);
	printf(1,"Calculation + IO Medium -  Wait time: %d,  Run time: %d, IO Time: %d\n\n",sum_wtime[2]/NUM_OF_CHILDS,sum_rtime[2]/NUM_OF_CHILDS,sum_iotime[2]/NUM_OF_CHILDS);
	printf(1,"Calculation + IO Large -  Wait time: %d,  Run time: %d, IO Time: %d\n\n",sum_wtime[3]/NUM_OF_CHILDS,sum_rtime[3]/NUM_OF_CHILDS,sum_iotime[3]/NUM_OF_CHILDS);

	exit();
}