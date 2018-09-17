//
//  pcb.c
//
//
//
//  Created by Matthew McAvey and Blake Eichenberger on 9/3/18.
//
//

#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#define NULL 0


=======
//GLOBAL VARIABLES


HIDDEN pcb_t* pcbList_h;

/* puts it onto the free list */
void freePcb (pcb_t* p){
    insertProcQ(*pcbList_h, p);
}

/* takes it off the free list*/
pcb_t* allocPcb (){

    pcb_t* allocPcbTemp = removeProcQ(&(pcbList_h));

    if (allocPcbTemp != NULL){
        allocPcbTemp->pnext = NULL;
        allocPcbTemp->pprnt = NULL;
        allocPcbTemp->pchild = NULL;
        allocPcbTemp->psib = NULL;
        allocPcbTemp->psemAdd = NULL; //maybe have to take it out
    }
    return (allocPcbTemp);

}

void initPcbs (){
    static pcb_t procTable[MAXPROC];
    int i;
    pcbList_h = mkEmptyProcQ();
    for (i=0; i<MAXPROC; i++){
        freePCB (&(procTable[i]));
    }
}

int emptyProcQ (pcb_PTR tp){
    return (tp == NULL);
}

void insertProcQ (pcb_t* *tp, pcb_t* p){
     if (emptyProcQ(*tp)){
         *tp = p;
         p->pnext = p;
         p->pprevious = p;
     } else {
         p->pnext = *tp->pnext;
         p->pprevious = *tp;
         *tp->next = *p;
         p->pnext->pprevious = p;
         *tp = p;
     }

}

pcb_t* removeProcQ (pcb_t* *tp){
    return(outProcQ(tp, *(tp->next)));

}
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

pcb_t* outProcQ (pcb_t* *tp, pcb_t* p){

    if (emptyProcQ(tp)){
        return (NULL);
    } else {
        if (*(tp->pnext) == *tp){
            if (*tp == p){
                pcb_t* temp = *tp;
                tp = NULL;
                return (temp);
            } else {
                return (NULL);
            }
        } else {
            pcb_t* target = *tp;
            while (*(target->pnext) != tp){
                if (*target->pnext == p){
                    *(p->pnext)->pprevious = *target;
                    *target->pnext = p->pnext;
                    return (p);
                }
            }
            if (*(target->pnext) == *p){
                *(p->pnext)->pprevious = *target;
                *target->pnext = p->pnext;
                return (p);
            }
            return (NULL);
        }
    }
}

pcb_t* headProcQ (pcb_t* tp){
    if (emptyProcQ(tp)){
        return (NULL);
    } else {
        return (*(tp->pnext));
    }
}
pcb_t* mkEmptyProcQ (){
    return (NULL);
}
/*tree methods****************************************************/

int emptyChild (pcb_t* p){
    return (*(p->pchild) == NULL);
}

void insertChild (pcb_t* prnt, pcb_t *p){
    if (prnt != NULL){
        p->psib = prnt->pchild;
        p->pprnt = prnt;
        prnt->pchild = *p;
    }
}

pcb_t* removeChild(pcb_t* p){
    if (emptyChild(p)){
        return (NULL);
    } else {
        pcb_t* temp = *(p->pchild);
        p->pchild = *(temp->psib);
        return (temp);
    }
}

pcb_t* outChild(pcb_t* p){
    if (*(p->pprnt) == NULL){
        return (NULL);
    } else {
        if (*(p->pprnt->pchild) == *p){
            return (removeChild(*(p->pprnt)));
        } else {
            pcb_t* target = *(p->parent->child);
            while (*(target->psib) != *p){
                target = *(target->psib);
            }
            target->psib = *(p->psib);
            return (p);
        }
    }
}

