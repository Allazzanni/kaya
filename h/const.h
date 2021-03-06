#ifndef CONSTS
#define CONSTS

/****************************************************************************
 *
 * This header file contains utility constants & macro definitions.
 *
 ****************************************************************************/

/* Hardware & software constants */
#define PAGESIZE		4096	/* page size in bytes */
#define WORDLEN			4		/* word size in bytes */
#define PTEMAGICNO		0x2A


#define ROMPAGESTART	0x20000000	 /* ROM Reserved Page */

/* definitions for the sys calls */
#define CREATEPROCESS 1
#define TERMINATEPROCESS 2
#define VERHOGEN 3
#define PASSEREN 4
#define SPECTRAPVEC 5
#define GETCPUTIME 8
#define WAITCLOCK 7
#define WAITIO 8

/* timer, timescale, TOD-LO and other bus regs */
#define RAMBASEADDR	0x10000000
#define TODLOADDR	0x1000001C
#define INTERVALTMR	0x10000020
#define TIMESCALEADDR	0x10000024

/* SYSYCALLS */
#define SYSCALLNEWAREA 0x200003D4
#define SYSCALLOLDAREA 0X20000348
/* Program trap */
#define PRGMTRAPNEWAREA 0x200002BC
#define PRGMTRAPOLDAREA 0x200001A4
/* Table management */
#define TBLMGMTNEWAREA 0x200001A4
#define TBLMGMTOLDAREA 0x20000118
/* Interrupt handling */
#define INTRUPTNEWAREA 0x2000008C
#define INTRUPTOLDAREA 0x20000000

/* utility constants */
#define	TRUE		1
#define	FALSE		0
#define ON              1
#define OFF             0
#define HIDDEN		static
#define EOS		'\0'

#define NULL ((void *)0xFFFFFFFF)


/* vectors number and type */
#define VECTSNUM	4

#define TLBTRAP		0
#define PROGTRAP	1
#define SYSTRAP		2

#define TRAPTYPES	3



/* device interrupts */
#define DISKINT		3
#define TAPEINT 	4
#define NETWINT 	5
#define PRNTINT 	6
#define TERMINT		7

#define DEVREGLEN	4	/* device register field length in bytes & regs per dev */
#define DEVREGSIZE	16 	/* device register size in bytes */

/* device register field number for non-terminal devices */
#define STATUS		0
#define COMMAND		1
#define DATA0		2
#define DATA1		3

/* device register field number for terminal devices */
#define RECVSTATUS      0
#define RECVCOMMAND     1
#define TRANSTATUS      2
#define TRANCOMMAND     3

#define IPAREA 0x0000FF00
#define IPONE 0x1
#define IPTWO 0x2
#define IPTHREE 0x4
#define IPFOUR 0x8
#define IPFIVE 0x10
#define IPSIX 0x20
#define IPSEVEN 0x40
#define IPEIGHT 0x80


/* device common STATUS codes */
#define UNINSTALLED	0
#define READY		1
#define BUSY		3

/* device common COMMAND codes */
#define RESET		0
#define ACK		1

/* operations */
#define	MIN(A,B)	((A) < (B) ? A : B)
#define MAX(A,B)	((A) < (B) ? B : A)
#define	ALIGNED(A)	(((unsigned)A & 0x3) == 0)

/* Useful operations */
#define STCK(T) ((T) = ((* ((cpu_t *) TODLOADDR)) / (* ((cpu_t *) TIMESCALEADDR))))
#define LDIT(T)	((* ((cpu_t *) INTERVALTMR)) = (T) * (* ((cpu_t *) TIMESCALEADDR)))


#endif
