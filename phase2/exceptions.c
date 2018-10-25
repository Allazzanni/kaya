

extern int processCount, softBlockCount;
extern pcb_t* currentProcess, readyQue;
extern int semaphores[SEMCOUNT];


syscallHandler (){
    state_t* oldState = (state_t*) 0x20000348
    /* this is a bit unnecasary but it is much easier to read if we do it */
    unsigned int status = oldState->s_status;
    unsigned int a0 = oldState->s_a0;
    
    if (a0 >= 9 && a0 < 255){
        passUpOrDie();
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
                    /*pass up or die*/
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
}

void sys1 (state_t* callerID){
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

void sys2 () {
    genocide (currentProcess);
    currentProcess = NULL;
    scheduler ();
}

void genocide (pcb_t* deadMan){
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

void sys3 (state_t* oldState){
    pcb_t* new = NULL;
    int* semAdd = oldState->s_a1;
    (*semAdd)++;
    if ((*semAdd) <= 0){
        new = removeBlocked(semAdd);
        new ->pcb_semAdd = NULL;
        insertProcQ(&(readyQueue), new);
    }
}

void sys4 (state_t* oldState){
    int* semAdd = oldState->s_a1;
    (*semAdd)--;
    if ((*semAdd) < 0){
        STCK(TODStopped);
        current = TODStopped - TODStarted;
        currentProcess->pcb_time = currentProcess->pcb_time + current;
        copyState(old, &(currentProcess->pcb_s));
        insertBlocked(semAdd, currentProcess);
        currentProcess = NULL;
        scheduler();
    }
}

void sys6 (state_t* doesNotOwnAWatch){
    int timeRemaining = 5000 - getTIMER();
    int clock = ;
    return clock + timeRemaining;
}

