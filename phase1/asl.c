

#include "../h/const.h"
#include "../h/types.h"
#include "../e/asl.e"
#include "../e/pcb.e"

HIDDEN semd_t* semd_h;
HIDDEN semd_t* semdFree_h;


semd_t* allocSemd (){
    if (semdFree_h == NULL){
        return NULL;
    }
    semd_t* temp = semdFree_h;
    semdFree_h = semdFree_h->snext;
    
    /* cleaning the dishes */
    temp->snext = NULL;
    temp->ssemd = NULL;
    temp->sprocq = NULL;
    
    return temp;
}

void freeSemd (semd_t* free){
    free->snext = semdFree_h;
    semdFree_h = free;
}

/* returns the semd before the target semAdd in the list, or if it is not there, the one before where it would go */
semd_t* getTarget(int *semAdd){
    /* sets target to the 0 dummy node */
    semd_t* target = semd_h;
    /* start by checking the node after the dummy node and every node after to find the one before our semAdd */
    while (*(target->snext->ssemd) > *semAdd){
        target = target->snext;
    }
    return target;
}

int insertBlocked (int *semAdd, pcb_t* p){
    semd_t* target = getTarget(semAdd);
    /* determines if the target semd is already there or if we need to add it */
    if (*(target->snext->ssemd) == semAdd){
        target = target-snext;
        pcb_t* *tp = &(target->sprocq);
        insertProcQ (tp, p);
        p->psemadd = semAdd;
        return (FALSE);
    } else {
        if (semdFree_h == NULL){
            return TRUE;
        }
        /* create the new semd_t and put it on the ASL where it should be
         aka the position after our current target */
        semd_t* newTarget = allocSemd();
        newTarget->snext = target->snext;
        target->snext = newTarget;
        newTarget->sprocq = mkEmptyProcQ();
        
        /* now we put the value in its pcbq */
        pcb_t* *tp = &(newTarget->sprocq);
        insertProcQ (tp, p);
        p->psemadd = semAdd;
        return FALSE;
    }
}

semd_t* removeBlocked (int *semAdd){
    semd_t* target = getTarget(semAdd);
    if (target->snext->ssemd == semAdd){
        pcb_t* temp = removeProcQ(&(target->snext->sprocq));
        if (emptyProcQ(target->snext->sprocq)){
            semd_t* tempSemd = target->snext;
            target->snext = target->snext->snext;
            freeSemd(tempSemd);
        }
        return temp;
    } else {
        return NULL;
    }
}

pcb_t* outBlocked (pcb_t* p){
    semd_t* target = getTarget (p->psemadd);
    if (target->snext->ssmed == p->psemadd){
        pcb_t* temp = outProcQ(&(target->snext->sprocq), p);
        if (temp == NULL){
            return NULL;
        } else {
            if (emptyProcQ(target->snext->sprocq)){
                semd_t* tempSemd = target->snext;
                target->snext = target->snext->snext;
                freeSemd(tempSemd);
            }
            return temp;
        }
    } else {
        return NULL;
    }
}

pcb_t* headBlocked (int* semAdd){
    semd_t* target = getTarget(*semAdd);
    if (target->snext->ssemd == semAdd){
        target = target->next;
        /* I am unsure if this is needed since there is no way for a semd
         to not have a procq, but it was required in the specs so I wrote it anyway */
        if (emptyProcQ(target->sprocq)){
            return NULL;
        } else {
            return headProcQ (target->sprocq);
        }
    } else {
        return NULL;
    }
}

/* inits with two dummy nodes on semd_h, one set to max int to mark the end and the other set to 0 to mark the start */
void initASL (){
    static semd_t semdTable[MAXPROC + 2];
    int i;
    semdFree_h = NULL;
    semd_h = NULL;
    for (i=0; i<MAXPROC; i++){
        freeSemd (&(semdTable[i]));
    }
    semd_h = &(semdtable[MAXPROC + 1]);
    semd_h->snext = &(semdTable[MAXPROC + 2]);
    semd_h->ssemd = 0;
    semd_h->snext->ssmed = (int*) MAX_INT;
    semd_h->snext->snext = NULL;
    semd_h->sprocq = NULL;
    semd_h->snext->sprocq = NULL;
    
}


