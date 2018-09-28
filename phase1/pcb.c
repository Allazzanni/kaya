/* PCB Queue and Tree */
/* the struct for pcb's are in types.h, the following is the implementation of the methods needed operate the pcbs as either a tree or a queue */
#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"

/* general advice for reading my code with regards to pointer variables:
 in general variables named target are used for iterating as the moving target through the list
 I dont drag a value behind when i iterate singly linked lists, instead I check the next value each time
 a variable named temp generally means I intend to return it or otherwise needed a temporary placeholder, sometimes target replaces this if i originially meant to iterate, and temp occasionally replaces target if I thought that I would be iterating and didnt end up doing so
 */

/*GLOBAL VARIABLES */


HIDDEN pcb_t* pcbList_h;

/* general pcb methods **********************************************************/

/* Insert the element pointed to by p onto the pcbFree list. */
void freePcb (pcb_t* p){
    p->pnext = pcbList_h;
    pcbList_h = p;
}

/* Return NULL if the pcbFree list is empty. Otherwise, remove an element from the pcbFree list, provide initial values for ALL of the ProcBlk’s fields (i.e. NULL and/or 0) and then return apointer to the removed element. ProcBlk’s get reused, so it is important that no previous value persist in a ProcBlk when it gets reallocated. */
pcb_t* allocPcb (){
    /* make sure the list isnt empty */
    if (emptyProcQ(pcbList_h)){
        return (NULL);
    }
    
    /* remove the next pcb and adjust the list */
    pcb_t *temp = pcbList_h;
    pcbList_h = pcbList_h->pnext;

    /* cleaning the dishes, aka reseting all the fields so we dont have wrong pointers */
    temp->pnext = NULL;
    temp->pprnt = NULL;
    temp->pchild = NULL;
    temp->psib = NULL;
    temp->psemadd = NULL;
    
    return (temp);

}

/* Initialize the pcbFree list to contain all the elements of thestatic array of MAXPROC ProcBlk’s. This method will be calledonly once during data structure initialization. */
void initPcbs (){
    static pcb_t procTable[MAXPROC];
    int i;
    pcbList_h = mkEmptyProcQ();
    for (i=0; i<MAXPROC; i++){
        freePcb (&(procTable[i]));
    }
}

/* Return TRUE if the queue whose tail is pointed to by tp is empty. Return FALSE otherwise. */
int emptyProcQ (pcb_t* tp){
    return (tp == NULL);
}

/* This method is used to initialize a variable to be tail pointer to a process queue. Return a pointer to the tail of an empty process queue; i.e. NULL. */
pcb_t* mkEmptyProcQ (){
    return (NULL);
}

/* queue methods ****************************************************************/

/* the queue is a doubly linked list with the pointer to the list pointing at the end value in the list */

/* Insert the ProcBlk pointed to by p into the process queue whose tail-pointer is pointed to by tp. Note the double indirection through tp to allow for the possible updating of the tail pointer as well. */
void insertProcQ (pcb_t* *tp, pcb_t* p){
    /*if the list is empty we make p the only value in the list */
     if (emptyProcQ(*tp)){
         *tp = p;
         p->pnext = p;
         p->pprevious = p;
     } else {
         /*otherwise we just put the value at the end of the list */
         p->pnext = (*tp)->pnext;
         p->pprevious = *tp;
         (*tp)->pnext = p;
         p->pnext->pprevious = p;
         *tp = p;
     }

}

/* Remove the first (i.e. head) element from the process queue whose tail-pointer is pointed to by tp. Return NULL if the process queue was initially empty; otherwise return the pointer to the removed element. Update the process queue’s tail pointer if necessary. */
pcb_t* removeProcQ (pcb_t* *tp){
    pcb_t* temp;
    /* start by checking if queue is empty */
    if (emptyProcQ(*tp)){
        return (NULL);
    } else {
        /* then we check if the list has only one value since we know it has at least one */
        if (*tp == (*tp)->pnext){
            temp = *tp;
            *tp = mkEmptyProcQ();
            return (temp);
        } else {
            /* if there are at least 2 values then we have a slightly different method to remove the first value */
            temp = (*tp)->pnext;
            /* this is a bit of a mess, but essentially its picking the value out of the spot it is in and then linking the surrounding values */
            (*tp)->pnext->pnext->pprevious = *tp;
            (*tp)->pnext = (*tp)->pnext->pnext;
            return (temp);
        }
    }
}

/* Remove the ProcBlk pointed to by p from the process queue whose tail-pointer is pointed to by tp. Update the process queue’s tail-pointer if necessary. If the desired entry is not in the indicated queue(an error condition), return NULL; otherwise, return p. Note that p can point to any element of the process queue. */
pcb_t* outProcQ (pcb_t* *tp, pcb_t* p){
    /* start by confirming that someone did not screw up */
    if (emptyProcQ(*tp) || p == NULL){
        /* if you are here you did not read the method description or are just really bad at pointers */
        return (NULL);
    } else {
        /* we start by checking if the list has only one value */
        if ((*tp)->pnext == *tp){
            /* if so then p has to be tp or we have a problem */
            if (*tp == p){
                pcb_t* temp = *tp;
                *tp = mkEmptyProcQ();
                return (temp);
            } else {
                return (NULL);
            }
        } else {
            /* now we know that at least two values are in the list so we iterate through to find p using a point labeled target */
            pcb_t* target = (*tp)->pnext;
            /* since we do not know the length of the list, the end condition is when we loop back to tp */
            while (target != *tp){
                /* if we find p we are done so it removes and returns here */
                if (target == p){
                    p->pnext->pprevious = target->pprevious;
                    target->pprevious->pnext = p->pnext;
                    return (target);
                }
                target = target->pnext;
            }
            /* this is here because the way the loop was written it would end on tp so we had to do one last check in case you asked me to remove tp. */
            if (target == p){
                p->pnext->pprevious = p->pprevious;
                p->pprevious->pnext = p->pnext;
                return (target);
            }
            return (NULL);
        }
    }
}

/* Return a pointer to the first ProcBlk from the process queue whose tail is pointed to by tp. Do not remove this ProcBlk from the process queue. Return NULL if the process queue is empty. */
pcb_t* headProcQ (pcb_t* tp){
    if (emptyProcQ(tp)){
        return (NULL);
    } else {
        return (tp->pnext);
    }
}

/*tree methods*******************************************************************/


/* Return TRUE if the ProcBlk pointed to by p has no children. Return FALSE otherwise. */
int emptyChild (pcb_t* p){
    return (p->pchild == NULL);
}

/* Make the ProcBlk pointed to by p a child of the ProcBlk pointed to by prnt. */
void insertChild (pcb_t* prnt, pcb_t *p){
    if (prnt != NULL){
        p->psib = prnt->pchild;
        p->pprnt = prnt;
        prnt->pchild = p;
    }
    /* the specifications did not have a response to what to do if prnt was NULL so I just left it undone */
}

/* Make the first child of the ProcBlk pointed to by p no longer a child of p. Return NULL if initially there were no children of p. Otherwise, return a pointer to this removed first child ProcBlk. */
pcb_t* removeChild(pcb_t* p){
    /* check if it has a kid */
    if (emptyChild(p)){
        return (NULL);
    } else {
        /* if so we draft the kid for the army... aka we remove it and return it */
        pcb_t* temp = p->pchild;
        p->pchild = temp->psib;
        return (temp);
    }
}

/* Make the ProcBlk pointed to byp no longer the child of its parent. If the ProcBlk pointed to by p has no parent, return NULL; otherwise,return p. Note that the element pointed to by p need not be the first child of its parent. */
pcb_t* outChild(pcb_t* p){
    /* idiot proofing */
    if (p->pprnt == NULL){
        /* you asked me to take an orphan away from his parents...*/
        return (NULL);
    } else {
        /* first condition is best condition, that is that p is the first child of its parent */
        if (p->pprnt->pchild == p){
            return (removeChild(p->pprnt));
        } else {
            /* since p has siblings we have go through them to find p and correctly remove it since the list is singly linked */
            pcb_t* target = p->pprnt->pchild;
            while (target->psib != p){
                target = target->psib;
            }
            target->psib = p->psib;
            return (p);
        }
    }
}

