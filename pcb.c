//
//  pcb.c
<<<<<<< HEAD
=======
//
>>>>>>> 250accc6106b964942dbcab327f8d1dfa9dfa423
//
//
//  Created by Matthew McAvey and Blake Eichenberger on 9/3/18.
//Last editted by Blake Eichenberger @11:30am

//

//#include "const.h"
#include "../h/types.h"
//#include "pcb.e"
#define NULL 0

<<<<<<< HEAD
=======
//GLOBAL VARIABLES
#include "const.h"
#include "types.h"
#include "pcb.e"
pcb_PTR pcbFree_h;
pcb_PTR pcbList_h;

/*process control block type*/
>>>>>>> 250accc6106b964942dbcab327f8d1dfa9dfa423

/* puts it onto the free list */
void freePcb (pcb_PTR p){

}

/* takes it off the free list*/
pcb_PTR allocPcb (){
<<<<<<< HEAD

=======
    pcb_PTR allocPcbTemp = removeProcQ(&(pcbList_h));

    if (allocPcbTemp != NULL){
        allocPcbTemp->pnext = NULL;
        allocPcbTemp->pprnt = NULL;
        allocPcbTemp->pchild = NULL;
        allocPcbTemp->psib = NULL;
        allocPcbTemp->psemAdd = NULL; //maybe have to take it out
    }
    return (allocPcbTemp);
>>>>>>> 250accc6106b964942dbcab327f8d1dfa9dfa423
}

void initPcbs (){
<<<<<<< HEAD

}
pcb_PTR emptyProcQ (pcb_PTR tp){
  if (tp =! NULL) {
    return(tp);

  }

}
void insertProcQ (pcb_PTR *tp, pcb_PTR p){

=======
    static pcb_t procTable[MAXPROC];
    pcbList_h = mkEmptyProcQ();
    for (int i=0; i<MAXPROC; i++;){
        freePcb (&(procTable[i]));
    }
}

int emptyProcQ (pcb_PTR tp){
    return (tp == NULL);
}

void insertProcQ (pcb_PTR *tp, pcb_PTR p){
    if (emptyProcQ(*tp)){
        *tp = p;
        p->pnext = p;
        p->pprevious = p;
    } else {
        p->pnext = *tp->pnext;
        p->pprevious = *tp;
        *tp->next = *p;
        p->pnext->pprevious = p;
        *tp = p; //when we come back i get to tell you i told you so
    }
>>>>>>> 250accc6106b964942dbcab327f8d1dfa9dfa423
}

pcb_PTR removeProcQ (pcb_PTR *tp){
<<<<<<< HEAD
    if (emptyProcQ(tp)){
        return (*tp=NULL);
    }

=======
    /*if (emptyProcQ(tp)){
        return (NULL);
    } else {
        if (tp == tp->pnext){
            pcb_PTR temp = tp->pnext;
            tp.next = NULL;
            return (temp);
        } else {
            pcb_PTR temp = tp->pnext;
            tp.next = tp->pnext->pnext;
            return (temp);
        }
    }*/
    return (outProcQ(tp, *tp->pnext));
>>>>>>> 250accc6106b964942dbcab327f8d1dfa9dfa423
}

pcb_PTR outProcQ (pcb_PTR *tp, pcb_PTR p){
<<<<<<< HEAD

=======
    if (emptyProcQ(tp)){
        return (NULL);
    } else {
        if (*tp->pnext == tp){
            if (*tp == p){
                pcb_PTR temp = *tp;
                tp = NULL;
                return (temp);
            } else {
                return (NULL);
            }
        } else {
            pcb_PTR target = tp;
            while (*target->pnext != tp){
                if (*target->pnext = p){
                    p->pprevious = *target;
                    *target->pnext = p->pnext;
                    return (p);
                }
            }
            return (NULL);
        }
    }
>>>>>>> 250accc6106b964942dbcab327f8d1dfa9dfa423
}

pcb_PTR headProcQ (pcb_PTR tp){
    if (emptyProcQ(tp)){
        return (NULL);
    } else {
<<<<<<< HEAD
        return (tp->pcb->next);
=======
        return (*tp->pnext);
>>>>>>> 250accc6106b964942dbcab327f8d1dfa9dfa423
    }
}
pcb_PTR mkEmptyProcQ (){
    return (NULL);
}
