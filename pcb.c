//
//  pcb.c
//
//
//  Created by Matthew McAvey and Blake Eichenberger on 9/3/18.
//Last editted by Blake Eichenberger @11:30am

//

//#include "const.h"
#include "../h/types.h"
//#include "pcb.e"
#define NULL 0


void freePcb (pcb_PTR p){

}
pcb_PTR allocPcb (){

}
void initPcbs (){

}
pcb_PTR emptyProcQ (pcb_PTR tp){
  if (tp =! NULL) {
    return(tp);

  }

}
void insertProcQ (pcb_PTR *tp, pcb_PTR p){

}
pcb_PTR removeProcQ (pcb_PTR *tp){
    if (emptyProcQ(tp)){
        return (*tp=NULL);
    }

}
pcb_PTR outProcQ (pcb_PTR *tp, pcb_PTR p){

}
pcb_PTR headProcQ (pcb_PTR tp){
    if (emptyProcQ(tp)){
        return (NULL);
    } else {
        return (tp->pcb->next);
    }
}
pcb_PTR mkEmptyProcQ (){
    return (NULL);
}
