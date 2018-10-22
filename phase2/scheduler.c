


#include "../h/const.h"
#include "../h/types.h"
#include "../e/initial.e"
#include "../e/pcb.e"
#include "/usr/local/include/umps2/umps/libumps.e"

extern int processCount, softBlockCount;
extern pcb_t* currentProcess, readyQue;

void scheduler () {
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
