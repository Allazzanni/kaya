

extern int processCount, softBlockCount;
extern pcb_t* currentProcess, readyQue;
extern int semaphores[SEMCOUNT];

HIDDEN void sys1(state_t* oldState);
HIDDEN void sys2();
HIDDEN void sys3(state_t* oldState);
HIDDEN void sys4(state_t* oldState);
HIDDEN void sys5(state_t* oldState);
HIDDEN void sys6(state_t* oldState);
HIDDEN void sys7(state_t* oldState);
HIDDEN void sys8(state_t* oldState);
HIDDEN void genocide(pcb_t* deadMan);
HIDDEN int findWaldo (state_t* oldState);
HIDDEN void theLordsJudgement(state_t* pagan, int plee);

void copyState(state_t* source, state_t* destination);

void tlbManager(){
    state_t* oldState = (state_t*) TBLMGMTOLDAREA;
    theLordsJudgement(oldState, TLBTRAP);
}

void pgmTrap(){
    state_t* oldState = (state_t*) PGMTRAPOLDAREA;
    theLordsJudgement(oldState, PROGTRAP);
}

syscallHandler (){
    state_t* oldState = (state_t*) 0x20000348
    /* this is a bit unnecasary but it is much easier to read if we do it */
    unsigned int status = oldState->s_status;
    unsigned int a0 = oldState->s_a0;
    
    if (a0 >= 9 && a0 < 255){
        theLordsJudgement();
    } else {
        if (/*status is in kernal mode */){
            oldState->s_pc = oldState->s_pc + 4;
            switch (a0) {
                case 1:
                    sys1(oldState);
                break;
                case 2:
                    sys2();
                break;
                case 3:
                    sys3(oldState);
                break;
                case 4:
                    sys4(oldState);
                break;
                case 5:
                    sys5(oldState);
                break;
                case 6:
                    sys6(oldState);
                break;
                case 7:
                    sys7(oldState);
                break;
                case 8:
                    sys8(oldState);
                break;
                default:
                    theLordsJudgement(oldState, SYSTRAP);
                break;
            }
        } else {
            /*priviledged instruction pgmtrap */
            state_t* pgTrap_s = (state_t*) 0x20000230
            copyState(oldState, pgTrap_s);
            pgTrap_s->s_cause = (unsigned int) 10;
            pgmTrap();
        }
    }
    PANIC();
}

HIDDEN void sys1 (state_t* callerID){
    p = allocPCB ();
    if (p == NULL){
        callerID->s_v0 = -1;
    } else {
        ++processCount;
        
        insertChild(currentProcess, p);
        
        copyState ((state_t *) callerID->s_a1, &(p->p_state));
        
        callerID->v0 = 0;
    }
    LDST (callerID);
}

HIDDEN void sys2 () {
    genocide (currentProcess);
    currentProcess = NULL;
    scheduler ();
}

HIDDEN void genocide (pcb_t* deadMan){
    if (currentProcess == deadMan){
        outChild(deadMan)
    } else {
        if (*(deadMan->psemadd) == 0){
            outProcQ(readyQue, deadMan);
        } else {
            outBlocked (deadMan);
            if (*(deadMan->psemadd) < SEMCOUNT){
                --softBlockCount;
            } else {
                --(*(deadMan->psemadd));
            }
        }
    }
    if (!emptyChild(deadMan)){
        genocide (deadMan->pchild);
    }
    if (deadMan->psib != NULL && deadMan != currentProcess){
        genocide (deadMan->psib);
    }
    freePCB (deadMan);
    --processCount;
}

HIDDEN void sys3 (state_t* oldState){
    pcb_t* new = NULL;
    int* semAdd = oldState->s_a1;
    (*semAdd)++;
    if ((*semAdd) <= 0){
        new = removeBlocked(semAdd);
        if (new != NULL){
            new ->psemadd = NULL;
            insertProcQ(&(readyQueue), new);
        }
    }
    LDST (oldState);
}

HIDDEN void sys4 (state_t* oldState){
    int* semAdd = oldState->s_a1;
    (*semAdd)--;
    if ((*semAdd) < 0){
        /* grant and chris's solution
        STCK(TODStopped);
        current = TODStopped - TODStarted;
        currentProcess->pcb_time = currentProcess->pcb_time + current;
        copyState(old, &(currentProcess->pcb_s));
        insertBlocked(semAdd, currentProcess);
        currentProcess = NULL;
        scheduler();
         */
        /* my solution */
        copyState(oldState, &(currentProcess->p_state));
        insertBlocked(semAdd, currentProcess);
        scheduler();
    }
    LDST(oldState);
}

HIDDEN void sys5 (state_t* oldState){
    switch (oldState->s_a1){
        case TLBTRAP:
            if (currentProcess->newTLB != NULL){
                sys2();
            }
            currentProcess->oldTLB = (state_t*) oldState->s_a2;
            currentProcess->newTLB = (state_t*) oldState->s_a3;
            break;
        case PROGTRAP:
            if (currentProcess->newPgm != NULL){
                sys2();
            }
            currentProcess->oldPgm = (state_t*) oldState->s_a2;
            currentProcess->newPgm = (state_t*) oldState->s_a3;
            break;
        case SYSTRAP:
            if (currentProcess->newSys != NULL){
                sys2();
            }
            currentProcess->oldSys = (state_t*) oldState->s_a2;
            currentProcess->newSys = (state_t*) oldState->s_a3;
            break;
    }
    LDST(oldState);
}

HIDDEN void sys6 (state_t* doesNotOwnAWatch){
    cpu_t stopTOD;
    STCK(stopTOD);
    cpu_t elapsedTime = stopTOD - startTOD;
    currentProcess->p_time = currentProcess->p_time + elapsedTime;
    doesNotOwnAWatch->s_v0 = currentProcess->p_time;
    STCK(startTOD);
    LDST(doesNotOwnAWatch);
}

HIDDEN void sys7 (state* oldState){
    int* semAdd = (int*) &(semaphores[SEMCOUNT-1]);
    (*semAdd)--;
    insertBlocked(semAdd, currentProcess);
    copyState(oldState, &(currentProcess->p_state));
    softBlockCount++;
    scheduler();
}

HIDDEN void sys8 (state* oldState){
    int waldo = findWaldo (oldState);
    int* semAdd = &(semaphores[waldo]);
    *semAdd--;
    if (semAdd < 0){
        insertBlocked (semAdd, currentProcess);
        copyState(oldState, &(currentProcess->p_state));
        softBlockCount++;
        scheduler();
    } else {
        PANIC();
    }
}

HIDDEN int findWaldo (state_t* oldState){
    int line, device, read, waldo;
    line = oldState->s_a1;
    device = oldState->s_a0;
    if (line < DISKINT || line > TERMINT){
        sys2()
    }
    /* basic equation is that we take the devices per int (DEVPERINT) and
     we multiply it by its device (removings the first few devices that do
     not have 8 semaphores and then add that to the device location and
     we find waldo */
    if (line == TERMINT && read == TRUE){
        waldo = DEVPERINT*(line - DEVNOSEM + read) + device;
    } else {
        waldo = DEVPERINT*(line - DEVNOSEM) + device;
    }
    return waldo;
}

/* passUpOrDie */
void theLordsJudgement(state_t* pagan, int plee) {
    switch (plee){
        case SYSTRAP:
            if (currentProcess->newSys != NULL){
                copyState(pagan, currentProcess->sysOld);
                LDST(currentProcess)
            }
            break;
        case TLBTRAP:
            if (currentProcess->newTLB != NULL){
                copyState(pagan, currentProcess->oldTLB);
                LDST(currentProcess->oldTLB);
            }
            break;
        case PROGTRAP:
            if (currentProcess->oldPgm != NULL){
                copyState(pagan, currentProcess->oldPgm);
                LDST (currentProcess->oldPgm);
            }
            break;
    }
    /* burn at the stake */
    sys2();
}

HIDDEN void copyState(state_t* source, state_t* destination){
    int i;
    destination->s_asid = source->s_asid;
    destination->s_status = source->s_status;
    destination->s_pc = source->s_pc;
    destination->s_cause = source->s_cause;
    for (i=0, i < STATEREGNUM, i++){
        destination->s_reg[i] = source->s_reg[i];
    }
}

