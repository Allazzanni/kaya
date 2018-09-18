
#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"



/*GLOBAL VARIABLES */


HIDDEN pcb_t* pcbList_h;

/* puts it onto the free list */
void freePcb (pcb_t* p){
    p->pnext = pcbList_h;
    pcbList_h = p;
}

/* takes it off the free list*/
pcb_t* allocPcb (){
    if (pcbList_h == NULL){
        return (NULL);
    }
    pcb_t *allocPcbTemp = pcbList_h;
    pcbList_h = pcbList_h->pnext;
    /*pcb_t* allocPcbTemp = removeProcQ(&(pcbList_h));*/

    
    allocPcbTemp->pnext = NULL;
    allocPcbTemp->pprnt = NULL;
    allocPcbTemp->pchild = NULL;
    allocPcbTemp->psib = NULL;
    allocPcbTemp->psemAdd = NULL;
    
    return (allocPcbTemp);

}

void initPcbs (){
    static pcb_t procTable[MAXPROC];
    int i;
    pcbList_h = mkEmptyProcQ();
    for (i=0; i<MAXPROC; i++){
        freePcb (&(procTable[i]));
    }
}

int emptyProcQ (pcb_t* tp){
    return (tp == NULL);
}

void insertProcQ (pcb_t* *tp, pcb_t* p){
     if (emptyProcQ(*tp)){
         *tp = p;
         p->pnext = p;
         p->pprevious = p;
     } else {
         p->pnext = (*tp)->pnext;
         p->pprevious = *tp;
         (*tp)->pnext = p;
         p->pnext->pprevious = p;
         *tp = p;
     }

}

pcb_t* removeProcQ (pcb_t* *tp){
    /*pcb_PTR ret;
    if(emptyProcQ(*tp)) {
        return NULL;
    } else if ((*tp) -> pnext == (*tp)) {
        ret = (*tp);
        (*tp) = mkEmptyProcQ();
        return ret;
    }
    ret  = (*tp) -> pnext;
    (*tp) -> pnext -> pnext -> pprevious = (*tp);
    (*tp) -> pnext = ((*tp) -> pnext -> pnext);
    return ret;*/
    return (outProcQ (tp, (*tp)->pnext));
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
    
    if (emptyProcQ(*tp) || p == NULL){
        return (NULL);
    } else {
        if ((*tp)->pnext == *tp){
            if (*tp == p){
                pcb_t* temp = *tp;
                tp = mkEmptyProcQ();
                return (temp);
            } else {
                return (NULL);
            }
        } else {
            pcb_t* target = (*tp)->pnext;
            while (target != *tp){
                if (target == p){
                    p->pnext->pprevious = target->pprevious;
                    target->pprevious->pnext = p->pnext;
                    return (target);
                }
                target = target->pnext;
            }
            if (target == p){
                p->pnext->pprevious = p->pprevious;
                p->pprevious->pnext = p->pnext;
                return (target);
            }
            return (NULL);
        }
    }
}

pcb_t* headProcQ (pcb_t* tp){
    if (emptyProcQ(tp)){
        return (NULL);
    } else {
        return (tp->pnext);
    }
}
pcb_t* mkEmptyProcQ (){
    return (NULL);
}
/*tree methods****************************************************/

int emptyChild (pcb_t* p){
    return (p->pchild == NULL);
}

void insertChild (pcb_t* prnt, pcb_t *p){
    if (prnt != NULL){
        p->psib = prnt->pchild;
        p->pprnt = prnt;
        *(prnt->pchild) = *p;
    }
}

pcb_t* removeChild(pcb_t* p){
    if (emptyChild(p)){
        return (NULL);
    } else {
        pcb_t* temp = p->pchild;
        p->pchild = temp->psib;
        return (temp);
    }
}

pcb_t* outChild(pcb_t* p){
    if (p->pprnt == NULL){
        return (NULL);
    } else {
        if (p->pprnt->pchild == p){
            return (removeChild(p->pprnt));
        } else {
            pcb_t* target = p->pprnt->pchild;
            while (target->psib != p){
                target = target->psib;
            }
            target->psib = p->psib;
            return (p);
        }
    }
}

