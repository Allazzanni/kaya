
extern int processCount, softBlockCount;
extern pcb_t* currentProcess, readyQue;
extern int semaphores[SEMCOUNT], psuedoClock;

extern void copyState (state_t* source, state_t* destination);
extern int findWaldo (int line, int device, int read)

void interuptHandler () {
    int line, read, status;
    
    
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
    line = line - DEVNOSEM;
    
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
    
    device_t* deviceRegister = (device_t*) (INTDEVREG + ((line - DEVNOSUM)*DEVREGSIZE*DEVPERINT) + (deviceNum*DEVREGSIZE));
    
    /* check if it is a terminal and adjust accordingly */
    if ((line + 3) != TERMINT){
        status = deviceRegister->d_status;
        deviceRegister->d_command = ACK;
    } else {
        read = (deviceRegister->t_transm_status & 0xFF);
        switch (read):
        case 3:
        case 4:
        case 5:
            index = (DEVPERINT *(line - DEVNOSEM)) + number;
            status = deviceRegister->t_recv_status;
            devReg->t_recv_command = ACK;
        break;
        default:
            /*adjust index*/
            status = deviceRegister->t_recv_status;
            deviceRegister->t_recv_command = ACK;
        break;
    }
    
    device_t theActualDevice = theThing->devreg[index];
    
    
    /* V operaton the semaphore */
    /*increment the semaphore */
    /* test if <= 0 */
        /*p = removeBlocked*/
        /* p->p_state.s_v0 = status of device out of device register */
        /*insertProcQ(p, readyQue)*/
        /*softBlockCount--;*/
    /*ack the interupt */
        /* setting the command field in the device register to ACK (1) */
    /* return control to the process that was executing at the the time of the interupt */
    /*LDST(oldState) */
    /* if in waitstate then cal scheduler */
    
    
    
}










