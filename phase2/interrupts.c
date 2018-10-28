
extern int processCount, softBlockCount;
extern pcb_t* currentProcess, readyQue;
extern int semaphores[SEMCOUNT], psuedoClock;

extern void copyState (state_t* source, state_t* destination);
extern int findWaldo (int line, int device, int read)

void interuptHandler () {
    int line;
    state_t* ourState = (state_t*) INTERUPTOLDAREA;
    unsigned int problemChild = *(outState->s_cause);
    problemChild = (problemChild & IPAREA) >> 8;
    
    if ((problemChild & IPONE) == IPONE){
        PANIC();
    } else if ((problemChild & IPTWO) == IPTWO){
        
    } else if ((problemChild & IPTHREE) == IPTHREE) {
        
    } else if ((problemChild & IPFOUR) == IPFOUR) {
        line = DISKINT;
    } else if ((problemChild & IPFIVE) == IPFIVE){
        line = TAPEINT;
    } else if ((problemChild & IPSIX) == IPSIX) {
        line = NETWINT;
    } else if ((problemChild & IPSEVEN) == IPSEVEN) {
        line = PRNTINT;
    } else if ((problemChild & IPEIGHT) == IPEIGHT){
        line = TERMINT;
    } else {
        PANIC ();
    }
    
    /* im tired so ill just say see the video part a and comment later */
    devregarea_t* theThing;
    theThing = (devregarea_t*) RAMBASEADDR;
    unsigned int theOTherThing;
    theOtherThing = theThing->interrupt_dev[line];
    
    int number;
    if ((theOtherThing & IPONE) != 0){
        number = 1;
    } else if ((theOtherThing & IPTWO) != 0){
        number = 2;
    } else if ((theOtherThing & IPTHREE) != 0) {
        number = 3;
    } else if ((theOtherThing & IPFOUR) != 0) {
        number = 4;
    } else if ((theOtherThing & IPFIVE) != 0){
        number = 5;
    } else if ((theOtherThing & IPSIX) != 0) {
        number = 6;
    } else if ((theOtherThing & IPSEVEN) != 0) {
        number = 7;
    } else if ((theOtherThing & IPEIGHT) != 0){
        number = 8;
    }
    
    /* pick up tommorow (i am towards the end of part a video */
    
}

