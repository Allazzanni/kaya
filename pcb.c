//
//  pcb.c
//  
//
//  Created by Matthew McAvey on 9/3/18.
//

/*process control block type*/
typedef struct pcb_t{
    /*process queue fields*/
    struct pcb_t* pnext,/*pointer to next entry*/ /*process tree fields*/ *pprnt, /*pointer to parent*/ *pchild, /*pointer to 1st child*/ *psib; /*pointer to sibling*/
    statetps; /*processor state*/
    int* psemAdd; /*pointer to sema4 on*/
    /*which process blocked*/
    /*plus other entries to be added later*/
    }pcb_t;
typedef struct pcb_PTR{
    pcb_t* pcb
}

void freePcb (pcb_PTR p){
    
}
pcb_PTR allocPcb (){
    
}
void initPcbs (){
    
}
int emptyProcQ (pcb_PTR tp){
    return (tp->pcb == NULL)
}
void insertProcQ (pcb_PTR *tp, pcd_PTR p){
    
}
pcb_PTR removeProcQ (pcb_PTR *tp){
    if (emptyProcQ(tp)){
        return (NULL)
    } else {
        if 
    }
}
pcb_PTR outProcQ (pcb_PTR *tp, pcb_PTR p){
    
}
pcb_PTR headProcQ (pcb_PTR tp){
    if (emptyProcQ(tp)){
        return (NULL)
    } else {
        return (tp->pcb->next)
    }
}
pcb_PTR mkEmptyProcQ (){
    return (NULL)
}
