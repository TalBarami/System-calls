#include "types.h"
#include "user.h"

#define LARGE_SIZED_LOOP_IO     3000
#define MEDIUM_SIZED_LOOP_IO    500
#define LARGE_SIZED_LOOP     1000000000
#define MEDIUM_SIZED_LOOP    500000
#define PROCESSES_COUNT      12


int performCalulcation(int count)
{
    int x = 0;
    for(int i=0 ; i < count; i++)
    {
	if(i == 5)
            x++;
	else 
            x+=i;
    }
    return x;
}

void peformCalculationAndIo(int count){
    for(int i=0 ; i < count; i++)
    {
        printf(1,"Hello my number is : %d!\n",i);
    }
}

int main(int argc, char *argv[])
{	
    int type = PROCESSES_COUNT / 4;
    int pid, wtime, rtime, iotime, j;
    int average[4][3];
    int results[PROCESSES_COUNT][4][3];
    int procs[PROCESSES_COUNT];
    
    for(int i=0; i<4; i++){
        for(int j=0; j<3; j++){
            average[i][j] = 0;
        }
    }
    
    int x = 0;
    for(int i = 0; i < PROCESSES_COUNT; i++){
        pid = fork();
        if(pid == 0){ // If child process
            set_priority((i % 3) + 1);
            if(i < type) { // Medium sized loop calculations
                x += performCalulcation(MEDIUM_SIZED_LOOP);
                if(x == 0){
                    printf(1, "x is 0!");
                }
                
                exit();
            } else if(i < 2 * type) { // Large sized loop calculations + IO
                x += performCalulcation(LARGE_SIZED_LOOP);
                if(x == 0){
                    printf(1, "x is 0!");
                }
                
                exit();
            } else if(i < 3 * type) { // Medium sized loop calculations + IO
                peformCalculationAndIo(MEDIUM_SIZED_LOOP_IO);
                exit();
            } else { // Large sized loop calculations + IO
                peformCalculationAndIo(LARGE_SIZED_LOOP_IO);
                exit();
            }
        } else {
            procs[i] = pid;
        }
    }
    
    for(int i=0; i<PROCESSES_COUNT; i++){
        wait2(procs[i], &wtime, &rtime, &iotime);
            if(i < type) { // Medium sized loop calculations
                j=0;
            } else if(i < 2 * type) { // Large sized loop calculations + IO
                j=1;
            } else if(i < 3 * type) { // Medium sized loop calculations + IO
                j=2;
            } else { // Large sized loop calculations + IO
                j=3;
            }
            average[j][0] = average[j][0] + wtime;
            average[j][1] = average[j][1] + rtime;
            average[j][2] = average[j][2] + iotime;
            
            results[i][j][0] = wtime;
            results[i][j][1] = rtime;
            results[i][j][2] = iotime;
    }
    
    for(int i=0; i<4; i++){
        for(int j=0; j<3; j++){
            average[i][j] = average[i][j] / PROCESSES_COUNT;
        }
    }
    printf(1, "\n\n");
    printf(1, "Medium sized loop calculations, wait time: %d, run time: %d, io time: %d\n", average[0][0], average[0][1], average[0][2]);
    printf(1, "Large sized loop calculations, wait time: %d, run time: %d, io time: %d\n", average[1][0], average[1][1], average[1][2]);
    printf(1, "Medium sized loop calculations + IO, wait time: %d, run time: %d, io time: %d\n", average[2][0], average[2][1], average[2][2]);
    printf(1, "Large sized loop calculations + IO, wait time: %d, run time: %d, io time: %d\n", average[3][0], average[3][1], average[3][2]);
    
    for(int i=0; i<PROCESSES_COUNT; i++){
        printf(1, "\n\n");
        printf(1, "PROCESS: %d\n", i);
        printf(1, "Medium sized loop calculations, wait time: %d, run time: %d, io time: %d\n", results[i][0][0], results[i][0][1], results[i][0][2]);
        printf(1, "Large sized loop calculations, wait time: %d, run time: %d, io time: %d\n", results[i][1][0], results[i][1][1], results[i][1][2]);
        printf(1, "Medium sized loop calculations + IO, wait time: %d, run time: %d, io time: %d\n", results[i][2][0], results[i][2][1], results[i][2][2]);
        printf(1, "Large sized loop calculations + IO, wait time: %d, run time: %d, io time: %d\n", results[i][3][0], results[i][3][1], results[i][3][2]);
    }
    exit();
}