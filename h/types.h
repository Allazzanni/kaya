#ifndef TYPES
#define TYPES

/****************************************************************************
 *
 * This header file contains utility types definitions.
 *
 ****************************************************************************/


typedef signed int cpu_t;


typedef unsigned int memaddr;

typedef struct pcb_t{
    /*process queue fields*/
    struct pcb_t *pnext,/*pointer to next entry*/
     /*process tree fields*/
    *pprnt, /*pointer to parent*/
    *pchild, /*pointer to 1st child*/
    *psib,
    *pprevious; /*pointer to sibling*/
    int* psemadd; /*pointer to sema4 on*/
     /*which process blocked*/
    /*plus other entries to be added later*/
    state_t* p_state, oldSys, newSys, oldPgm, newPgm, oldTLB, newTLB;
    cpu_t* p_time;
  }pcb_t, *pcb_PTR;

typedef struct semd_t {
    struct semd_t *snext;
    int *ssemd;
    pcb_t *sprocq;
}semd_t;

typedef struct {
	unsigned int d_status;
	unsigned int d_command;
	unsigned int d_data0;
	unsigned int d_data1;
} device_t;

#define MAXPROC 20

#define t_recv_status		d_status
#define t_recv_command		d_command
#define t_transm_status		d_data0
#define t_transm_command	d_data1

#define DEVINTNUM 5
#define DEVPERINT 8
/*since the first three devices do not have 8 semaphores,
 This is needed in calculations that also use the above 2 so
 I put it here */
#define DEVNOSEM 3
typedef struct {
	unsigned int rambase;
	unsigned int ramsize;
	unsigned int execbase;
	unsigned int execsize;
	unsigned int bootbase;
	unsigned int bootsize;
	unsigned int todhi;
	unsigned int todlo;
	unsigned int intervaltimer;
	unsigned int timescale;
	unsigned int inst_dev[DEVINTNUM];
	unsigned int interrupt_dev[DEVINTNUM];
	device_t   devreg[DEVINTNUM * DEVPERINT];
} devregarea_t;

#define STATEREGNUM	31


typedef struct state_t {
	unsigned int	s_asid;
	unsigned int	s_cause;
	unsigned int	s_status;
	unsigned int 	s_pc;
	int	 			s_reg[STATEREGNUM];

} state_t, *state_PTR;

#define	s_at	s_reg[0]
/* returns information to the calling process */
#define	s_v0	s_reg[1]
#define s_v1	s_reg[2]
/* a registers, relevant for syscalls */
#define s_a0	s_reg[3]
#define s_a1	s_reg[4]
#define s_a2	s_reg[5]
#define s_a3	s_reg[6]
#define s_t0	s_reg[7]
#define s_t1	s_reg[8]
#define s_t2	s_reg[9]
#define s_t3	s_reg[10]
#define s_t4	s_reg[11]
#define s_t5	s_reg[12]
#define s_t6	s_reg[13]
#define s_t7	s_reg[14]
#define s_s0	s_reg[15]
#define s_s1	s_reg[16]
#define s_s2	s_reg[17]
#define s_s3	s_reg[18]
#define s_s4	s_reg[19]
#define s_s5	s_reg[20]
#define s_s6	s_reg[21]
#define s_s7	s_reg[22]
#define s_t8	s_reg[23]
#define s_t9	s_reg[24]
#define s_gp	s_reg[25]
#define s_sp	s_reg[26] /* stag pointer */
#define s_fp	s_reg[27]
#define s_ra	s_reg[28]
#define s_HI	s_reg[29]
#define s_LO	s_reg[30]


#endif
