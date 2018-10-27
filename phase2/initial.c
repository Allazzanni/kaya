

/* Global variables:
        ProcessCount (int)
        softBlockCount (int)
        currentProcess (pcb_t*)
        readyQue (pcb_t*)
 */
int processCount, softBlockCount;
pcb_t* currentProcess, readyQue;
int semaphores[SEMCOUNT];

/*where the booting happens */
main (){
    /* populate the four new areas in low memory
        set the stack pointer (last page of physical)
        set PC
        set the status: VM: off
                    Interupts masked
                    Supervisor mode on
     */
    unsigned int topOfPM;
    state_t* stateSetter;
    devregarea_t* pmInfo = (devregarea_t*) 0x10000000;
    
    topOfPM = (pmInfo->rambase) + (pmInfo->ramsize);
    
    /* syscallHandler */
    stateSetter = (state_t*) SYSCALLNEWAREA;
    stateSetter->s_status = ALLOFF;
    stateSetter->s_sp = topOfPM;
    stateSetter->s_pc = stateSetter->s_t9 = (memaddr) syscallHandler;
    
    /* pgmTrap */
    stateSetter = (state_t*) PRGMTRAPNEWAREA;
    stateSetter->s_status = ALLOFF;
    stateSetter->s_sp = topOfPM;
    stateSetter->s_pc = stateSetter->s_t9 = (memaddr) pgmTrap;
    
    /* tlbManager */
    stateSetter = (state_t*) TLBMGMTNEWAREA;
    stateSetter->s_status = ALLOFF;
    stateSetter->s_sp = topOfPM;
    stateSetter->s_pc = stateSetter->s_t9 = (memaddr) tlbManager;
    
    /* interuptHandler */
    stateSetter = (state_t*) INTERUPTNEWAREA;
    stateSetter->s_status = ALLOFF;
    stateSetter->s_sp = topOfPM;
    stateSetter->s_pc = stateSetter->s_t9 = (memaddr) interuptHandler;
    
    /* initialize phase1 */
    initPcbs();
    initASL();
    
    /* initialize phase 2 global variables */
    processCount = 0;
    softBlockCount = 0;
    currentProcess = NULL;
    readyQue = mkEmptyProcQ();
     /*
     p = allocatePCB ()
        initialize the p_s
            stack pointer to the penultimate page of physical memory
            PC (p2test)
            status
                VM off
                Interupts enabled
                supervisor mode on
     */
    currentProcess = allocPCB();
    currentProcess->p_state.s_sp = (topOfPM - PAGESIZE);
    currentProcess->p_state.s_pc = currentProcess->p_state.s_t9 = (memaddr) test;
    currentProcess->p_state.s_status = ALLOFF | ;
    
    int i = 0;
    while (i < SEMCOUNT){
        semaphores[i] = 0;
        i++;
    }
    
    LDIT(100000);
    
    processCount++;
    insertProcQ(&readyQue, currentProcess);
    scheduler();
}
