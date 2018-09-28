/* ASL maintanence and creation */
/* ASL's are built on the struct semd_t which is defined in types.h. The following is an implementation of the needed methods to operate and create an asl */

#include "../h/const.h"
#include "../h/types.h"
#include "../e/asl.e"
#include "../e/pcb.e"

/* general advice for reading my code with regards to pointer variables:
 in general variables named target are used for iterating as the moving target through the list
 I dont drag a value behind when i iterate singly linked lists, instead I check the next value each time
 a variable named temp generally means I intend to return it or otherwise needed a temporary placeholder, sometimes target replaces this if i originially meant to iterate, and temp occasionally replaces target if I thought that I would be iterating and didnt end up doing so
 */

/*GLOBAL VARIABLES */
/* the currently active semaphore list */
HIDDEN semd_t* semd_h;
/* the available semaphores*/
HIDDEN semd_t* semdFree_h;

/*the methods *******************************************************************************/

/* Return NULL if the semdFree-H list is empty. Otherwise, remove and element from said list and then provide initial values for all of the semd_t's fields (i.e NULL and/or 0) and then return a pointer to the removed element. */
semd_t* allocSemd (){
    if (semdFree_h == NULL){
        return NULL;
    }
    semd_t* temp = semdFree_h;
    semdFree_h = semdFree_h->snext;
    
    /* cleaning the dishes, aka setting the semd_t fields to intial values */
    temp->snext = NULL;
    temp->ssemd = NULL;
    temp->sprocq = NULL;
    
    return temp;
}

/* Insert the element pointed to by free onto the semdFree_h list */
void freeSemd (semd_t* free){
    free->snext = semdFree_h;
    semdFree_h = free;
}

/* wrote this to iterate through the semd_h list since it was essentially the same thing for several locations */
/* returns the semd before the target semAdd in the list, or if it is not there, the one before where it would go */
semd_t* getTarget(int *semAdd){
    /* sets target to the 0 dummy node */
    semd_t* target = semd_h;
    /* start by checking the node after the dummy node and every node after to find the one before our semAdd */
    while (target->snext->ssemd < semAdd){
        target = target->snext;
    }
    return target;
}

/* Insert the ProcBlk pointed to by p at the tail of the process queue associated with the semaphore whose physical address is semAdd and set the semaphore address of p to semAdd. If the semaphore is currently not active (i.e. there is no descriptor for it in the ASL), allocate a new descriptor from the semdFree list, insert it in the ASL (at the appropriate position), initialize all of the fields (i.e. set ssemAdd to semAdd, and sprocq to mkEmptyProcQ()), and proceed as above. If a new semaphore descriptor needs to be allocated and the semdFree list is empty, return TRUE. In all other cases return FALSE.*/
int insertBlocked (int *semAdd, pcb_t* p){
    /* gives us the semaphore on the active list with the highest address lower than semAdd (i.e. the value right before where our new semaphore will go) */
    semd_t* target = getTarget(semAdd);
    /* determines if the target semd is already there or if we need to add it */
    if (target->snext->ssemd == semAdd){
        /* if it is already there, then this is easy */
        target = target->snext;
        /* this seems minor, but i spent a lot of time bug testing this so i made a note here to make sure anyone touching this code remembers to that the pcbs have an address too, and they need it, and I wil be copying it wherever this is done */
        p->psemadd = semAdd;
        insertProcQ (&(target->sprocq), p);
        return (FALSE);
    } else {
        /* we dont have the desired semaphore so we must create the new semd_t and put it on the ASL where it should be aka the position after our current target */
        semd_t* newTarget = allocSemd();
        /* if this fails then we are out of semaphores and therefore out of luck */
        if (newTarget == NULL){
            return TRUE;
        }
        newTarget->snext = target->snext;
        target->snext = newTarget;
        newTarget->sprocq = mkEmptyProcQ();
        
        /* now we put the value in its pcbQ */
        insertProcQ (&(newTarget->sprocq), p);
        newTarget->ssemd = semAdd;
         /* this seems minor, but i spent a lot of time bug testing this so i made a note here to make sure anyone touching this code remembers to that the pcbs have an address too, and they need it, and I wil be copying it wherever this is done */
        p->psemadd = semAdd;
        return FALSE;
    }
}

/* Search the ASL for a descriptor of this semaphore.  If none is found, return NULL; otherwise, remove the first (i.e. head) ProcBlk from the process queue of the found semaphore descriptor and return a pointer to it. If the process queue for this semaphore becomes empty (emptyProcQ(sprocq) is TRUE), remove the semaphore descriptor from the ASL and return it to the semdFree list. */
pcb_t* removeBlocked (int *semAdd){
    /* gives us the semaphore on the active list with the highest address lower than semAdd (i.e. hopefully the value before where our target is) */
    semd_t* target = getTarget(semAdd);
    /* idiot proofing */
    if (target->snext->ssemd == semAdd){
        /* removes the desired pcb */
        pcb_t* temp = removeProcQ(&(target->snext->sprocq));
        /* if the semaphore is now empty we kill it */
        if (emptyProcQ(target->snext->sprocq)){
            semd_t* tempSemd = target->snext;
            target->snext = target->snext->snext;
            freeSemd(tempSemd);
        }
        return temp;
    } else {
        /* the address must have a correlating semaphore for us to remove said value */
        return NULL;
    }
}

/* Remove the ProcBlk pointed to by p from the process queue associated with p’s semaphore (p→psemAdd) on the ASL. If ProcBlk pointed to by p does not appear in the process queue associated with p’s semaphore, which is an error condition, return NULL; otherwise, return p. */
pcb_t* outBlocked (pcb_t* p){
    semd_t* target = getTarget (p->psemadd);
    /* idiot proofing */
    if (target->snext->ssemd == p->psemadd){
        /* removes our desired pcb from the list it should be on */
        pcb_t* temp = outProcQ(&(target->snext->sprocq), p);
        /* if this fails then you created a pcb with an address that correlates to an active sempahore, then didnt put that pcb in that semamphore... why? */
        if (temp == NULL){
            /* You had to do something manually to get here so its either a good test case or a really bad implementation */
            return NULL;
        } else {
            /* anytime we remove a pcb from a semaphore we have to ensure that the semaphore still has a pcb, if not we remove it from the active semaphores list */
            if (emptyProcQ(target->snext->sprocq)){
                semd_t* tempSemd = target->snext;
                target->snext = target->snext->snext;
                freeSemd(tempSemd);
            }
            return temp;
        }
    } else {
        /* what did you do, assign a psemadd without putting said p in a semaphore and then check if it was in the semaphores, why would you do that you masochist */
        return NULL;
    }
}

/* Return a pointer to the ProcBlk that is at the head of the process queue associated with the semaphore semAdd. Return NULL if semAdd is not found on the ASL or if the process queue associated with semAdd is empty. */
pcb_t* headBlocked (int* semAdd){
    semd_t* target = getTarget(semAdd);
    /* checks if the requested semAdd is even active */
    if (target->snext->ssemd == semAdd){
        target = target->snext;
        /* if this fails it means you have an active semaphore with an empty proc q or you ran headBlocked on a dummy node. dont do that. */
        if (emptyProcQ(target->sprocq)){
            /* I should remove the sempahore if it was up to me, but the specs said to return NULL, so thats what I did. This should never happen anyway */
            return NULL;
        } else {
            /* success */
            return headProcQ (target->sprocq);
        }
    } else {
        /* if you are here you are a test case or you have tried to headBlocked an address with no correlating semaphore */
        return NULL;
    }
}
/*official specification: */
/* Initialize the semdFree list to contain all the elements of the arraystaticsemdt semdTable[MAXPROC]This method will be only called once during data structure initializa-tion. */
/* important note from the author */
/* inits with two dummy nodes on semd_h, one set to max int to mark the end and the other set to 0 to mark the start */
void initASL (){
    static semd_t semdTable[MAXPROC + 2];
    int i;
    semdFree_h = NULL;
    semd_h = NULL;
    for (i=0; i<MAXPROC; i++){
        freeSemd (&(semdTable[i]));
    }
    /* the dummy nodes */
    semd_h = &(semdTable[MAXPROC]);
    semd_h->snext = &(semdTable[MAXPROC + 1]);
    /* end of dummy nodes */
    /*set the first dummy to 0 */
    semd_h->ssemd = 0;
    /* I couldnt find a keyword for max int, so I just looked up c's max int */
    semd_h->snext->ssemd = (unsigned int*) 2147483647; /* second dummy set to max int */
    semd_h->snext->snext = NULL;
    /* the dummys are the only ones allowed to not have a pcblist */
    semd_h->sprocq = NULL;
    semd_h->snext->sprocq = NULL;
}
