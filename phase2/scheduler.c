


#include "../h/const.h"
#include "../h/types.h"
#include "../e/initial.e"
#include "../e/pcb.e"
#include "/usr/local/include/umps2/umps/libumps.e"

cpu_t startTOD;

extern int processCount, softBlockCount;
extern pcb_t* currentProcess, readyQue;
extern int semaphores[SEMCOUNT];

void scheduler () {
    
    if (currentProcess != NULL){
        STCK(startTOD);
    }
    
    if (emptyProcQ(readyQue)){
        if (processCount == 0){
            HALT();
        }
        if (processCount > 0){
            if (softBlockCount == 0){
                PANIC ();
            }
            if (softBlockCount > 0){
                WAIT();
            }
    } else {
        currentProcess = removeProcQ(readyQue);
        /* lead a timer with the value of a quantum */
        setTIMER(5000);
        LDST(&(currentProcess->p_state));
    }
}
