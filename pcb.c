//
//  pcb.c
//  
//
//  Created by Matthew McAvey on 9/3/18.
//

/*process control block type*/

void freePcb (pcb_PTR p){
    
}

pcb_PTR allocPcb (){
    
}

void initPcbs (){
    
}

int emptyProcQ (pcb_PTR tp){
    return (tp == NULL);
}

void insertProcQ (pcb_PTR *tp, pcd_PTR p){
    if (emptyProcQ(tp)){
        tp = p;
    } else {
        p->next = tp->pnext;
        tp.next = *p;
    }
}

pcb_PTR removeProcQ (pcb_PTR *tp){
    if (emptyProcQ(tp)){
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
    }
}

pcb_PTR outProcQ (pcb_PTR *tp, pcb_PTR p){
    
}

pcb_PTR headProcQ (pcb_PTR tp){
    if (emptyProcQ(tp)){
        return (NULL);
    } else {
        return (tp->pcb->pnext);
    }
}
pcb_PTR mkEmptyProcQ (){
    return (NULL)
}
