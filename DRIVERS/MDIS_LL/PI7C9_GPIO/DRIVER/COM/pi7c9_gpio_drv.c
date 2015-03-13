/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  pi7c9_gpio_drv.c
 *
 *      \author  km
 *        $Date: 2012/08/02 12:42:54 $
 *    $Revision: 1.2 $
 *
 *      \brief   Low-level driver for Pericom PI7C9X442SL GPIO access 
 *
 *     Required: OSS, DESC, DBG, libraries
 *
 *     \switches _ONE_NAMESPACE_PER_DRIVER_, WDOG, WDOG_ONLY
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: pi7c9_gpio_drv.c,v $
 * Revision 1.2  2012/08/02 12:42:54  ts
 * R: previously set Registers were overwritten when other ports were set
 * M: corrected algorithm for masking the port bits
 *
 * Revision 1.1  2012/07/17 13:39:43  ts
 * Initial Revision
 *
 *----------------------------------------------------------------------------
 * (c) Copyright 2012 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#define _NO_LL_HANDLE		/* ll_defs.h: don't define LL_HANDLE struct */

#include <MEN/men_typs.h>   /* system dependent definitions   */
#include <MEN/maccess.h>    /* hw access macros and types     */
#include <MEN/dbg.h>        /* debug functions                */
#include <MEN/oss.h>        /* oss functions                  */
#include <MEN/desc.h>       /* descriptor functions           */
#include <MEN/mdis_api.h>   /* MDIS global defs               */
#include <MEN/mdis_com.h>   /* MDIS common defs               */
#include <MEN/mdis_err.h>   /* MDIS error codes               */
#include <MEN/ll_defs.h>    /* low-level driver definitions   */
#include <MEN/pi7c9_gpio_drv.h>    /* definitions of the PI7C9 GPIOs */

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/* general defines */
#define CH_NUMBER			1			/**< Number of device channels */
#define USE_IRQ				FALSE		/**< Interrupt required  */
#define ADDRSPACE_COUNT		1			/**< Number of required address spaces */
#define ADDRSPACE_SIZE		1			/**< Size of address space */

/* debug defines */
#define DBG_MYLEVEL			llHdl->dbgLevel   /**< Debug level */
#define DBH					llHdl->dbgHdl     /**< Debug handle */

/* register offsets within PCI-Config-Space of PI7C9 */
#define GPIO_REG		0xD8	/**< GPIO register within PCI config space (32-bit) */
#define GPIO_COUNT		8

/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/** low-level handle */
typedef struct {
	/* general */
    int32           memAlloc;		/**< Size allocated for the handle */
    OSS_HANDLE      *osHdl;         /**< OSS handle */
    DESC_HANDLE     *descHdl;       /**< DESC handle */
    MACCESS         maGpio;         /**< HW access handle for GPIO space */
	MDIS_IDENT_FUNCT_TBL idFuncTbl;	/**< ID function table */
	/* debug */
    u_int32         dbgLevel;		/**< Debug level */
	DBG_HANDLE      *dbgHdl;        /**< Debug handle */
	/* misc */
    u_int32         pciBus;         /**< PCI bus */
    u_int32         pciDev;         /**< PCI dev */
    u_int32         pciFunc;        /**< PCI func */
    int32			DevID;
} LL_HANDLE;

/* include files which need LL_HANDLE */
#include <MEN/ll_entry.h>   	/* low-level driver jump table  */
#include <MEN/pi7c9_gpio_drv.h> /* PI7C9_GPIO driver header file */

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
static int32 PI7C9_GPIO_Init(DESC_SPEC *descSpec, OSS_HANDLE *osHdl,
					         MACCESS *ma, OSS_SEM_HANDLE *devSemHdl,
					         OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP);
static int32 PI7C9_GPIO_Exit(LL_HANDLE **llHdlP);
static int32 PI7C9_GPIO_Read(LL_HANDLE *llHdl, int32 ch, int32 *value);
static int32 PI7C9_GPIO_Write(LL_HANDLE *llHdl, int32 ch, int32 value);
static int32 PI7C9_GPIO_SetStat(LL_HANDLE *llHdl,int32 ch, int32 code,
								INT32_OR_64 value32_or_64);
static int32 PI7C9_GPIO_GetStat(LL_HANDLE *llHdl, int32 ch, int32 code,
								INT32_OR_64 *value32_or_64P);
static int32 PI7C9_GPIO_BlockRead(LL_HANDLE *llHdl, int32 ch, void *buf,
								  int32 size, int32 *nbrRdBytesP);
static int32 PI7C9_GPIO_BlockWrite(LL_HANDLE *llHdl, int32 ch, void *buf,
								   int32 size, int32 *nbrWrBytesP);
static int32 PI7C9_GPIO_Irq(LL_HANDLE *llHdl );
static int32 PI7C9_GPIO_Info(int32 infoType, ... );

static char* Ident( void );
static int32 Cleanup(LL_HANDLE *llHdl, int32 retCode);


/****************************** PI7C9_GPIO_GetEntry ********************************/
/** Initialize driver's jump table
 *
 *  \param drvP     \OUT Pointer to the initialized jump table structure
 ***********************************************************************************/
#ifdef _ONE_NAMESPACE_PER_DRIVER_
    extern void LL_GetEntry( LL_ENTRY* drvP )
#else
    extern void __PI7C9_GPIO_GetEntry( LL_ENTRY* drvP )
#endif /* _ONE_NAMESPACE_PER_DRIVER_ */
{
    drvP->init        = PI7C9_GPIO_Init;
    drvP->exit        = PI7C9_GPIO_Exit;
    drvP->read        = PI7C9_GPIO_Read;
    drvP->write       = PI7C9_GPIO_Write;
    drvP->blockRead   = PI7C9_GPIO_BlockRead;
    drvP->blockWrite  = PI7C9_GPIO_BlockWrite;
    drvP->setStat     = PI7C9_GPIO_SetStat;
    drvP->getStat     = PI7C9_GPIO_GetStat;
    drvP->irq         = PI7C9_GPIO_Irq;
    drvP->info        = PI7C9_GPIO_Info;
}

/****************************** PI7C9_GPIO_Init ************************************/
/** Allocate and return low-level handle, initialize hardware
 *
 * The function initializes the HW with the definitions made
 * in the descriptor.
 *
 * The function decodes \ref descriptor_entries "these descriptor entries"
 * in addition to the general descriptor keys.
 *
 *  \param descP      \IN  Pointer to descriptor data
 *  \param osHdl      \IN  OSS handle
 *  \param ma         \IN  HW access handle
 *  \param devSemHdl  \IN  Device semaphore handle
 *  \param irqHdl     \IN  IRQ handle
 *  \param llHdlP     \OUT Pointer to low-level driver handle
 *
 *  \return           \c 0 On success or error code
 ***********************************************************************************/
static int32 PI7C9_GPIO_Init(
    DESC_SPEC       *descP,
    OSS_HANDLE      *osHdl,
    MACCESS         *ma,
    OSS_SEM_HANDLE  *devSemHdl,
    OSS_IRQ_HANDLE  *irqHdl,
    LL_HANDLE       **llHdlP
)
{
    LL_HANDLE *llHdl = NULL;
    u_int32 gotsize;
    int32	error;
    u_int32	value;

    /*------------------------------+
    |  prepare the handle           |
    +------------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */

	/* alloc */
    if ((llHdl = (LL_HANDLE*)OSS_MemGet(
    				osHdl, sizeof(LL_HANDLE), &gotsize)) == NULL)
       return(ERR_OSS_MEM_ALLOC);

	/* clear */
    OSS_MemFill(osHdl, gotsize, (char*)llHdl, 0x00);

	/* init */
    llHdl->memAlloc   = gotsize;
    llHdl->osHdl      = osHdl;

	llHdl->pciBus	= MAC_MAHDL2PCI_BUS(*ma);
	llHdl->pciDev	= MAC_MAHDL2PCI_DEV(*ma);
	llHdl->pciFunc	= MAC_MAHDL2PCI_FUNC(*ma);

    /*------------------------------+
    |  init id function table       |
    +------------------------------*/
	/* driver's ident function */
	llHdl->idFuncTbl.idCall[0].identCall = Ident;
	/* library's ident functions */
	llHdl->idFuncTbl.idCall[1].identCall = DESC_Ident;
	llHdl->idFuncTbl.idCall[2].identCall = OSS_Ident;
	/* terminator */
	llHdl->idFuncTbl.idCall[3].identCall = NULL;

    /*------------------------------+
    |  prepare debugging            |
    +------------------------------*/
	DBG_MYLEVEL = OSS_DBG_DEFAULT;	/* set OS specific debug level */
	DBGINIT((NULL,&DBH));

    /*------------------------------+
    |  scan descriptor              |
    +------------------------------*/
	/* prepare access */
    if ((error = DESC_Init(descP, osHdl, &llHdl->descHdl)))
		return( Cleanup(llHdl,error) );

    /* DEBUG_LEVEL_DESC */
    if ((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
								&value, "DEBUG_LEVEL_DESC")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

	DESC_DbgLevelSet(llHdl->descHdl, value);	/* set level */

    /* DEBUG_LEVEL */
    if ((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
								&llHdl->dbgLevel, "DEBUG_LEVEL")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

    DBGWRT_1((DBH, "LL - PI7C9_GPIO_Init\n"));

    /*------------------------------+
    |  init hardware                |
    +------------------------------*/
	/* nothing to do */

	*llHdlP = llHdl;	/* set low-level driver handle */

	return(ERR_SUCCESS);
}

/****************************** PI7C9_GPIO_Exit ************************************/
/** De-initialize hardware and clean up memory
 *
 *  The function deinitializes the HW, the watchdog will be disabled.
 *
 *  \param llHdlP	\IN  Pointer to low-level driver handle
 *
 *  \return			\c 0 On success or error code
 ***********************************************************************************/
static int32 PI7C9_GPIO_Exit(
   LL_HANDLE    **llHdlP
)
{
    LL_HANDLE *llHdl = *llHdlP;
	int32 error = 0;

    DBGWRT_1((DBH, "LL - PI7C9_GPIO_Exit\n"));

    /*------------------------------+
    |  de-init hardware             |
    +------------------------------*/

    /*------------------------------+
    |  clean up memory              |
    +------------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */
	error = Cleanup(llHdl,error);

	return(error);
}

/****************************** PI7C9_GPIO_Read ************************************/
/** Read a value from the device
 *
 *  The function reads the current state of all port pins.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param ch         \IN  Current channel
 *  \param valueP     \OUT Read value
 *
 *  \return           \c   ERR_LL_ILL_FUNC
 ***********************************************************************************/
static int32 PI7C9_GPIO_Read(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 *valueP
)
{
	int32		i, reg, error=0,mask=0;

    DBGWRT_1((DBH, "LL - PI7C9_GPIO_Read: ch=%d\n",ch));

	if ( (error = OSS_PciGetConfig(
		llHdl->osHdl, llHdl->pciBus, llHdl->pciDev, llHdl->pciFunc,
		OSS_PCI_ACCESS_32 | GPIO_REG, &reg))
	) return error;

	for( i=0; i<GPIO_COUNT; i++ )
		mask |= ((reg >> (i*4)) & 0x01)	<< i;

	*valueP = mask;
	DBGWRT_2((DBH, "PI7C9_GPIO_Read: valueP = 0x%02x\n",mask));
	return(ERR_SUCCESS);
}

/****************************** PI7C9_GPIO_Write ***********************************/
/** Description:  Write a value to the device
 *
 *  The function writes a value to the ports which are programmed as outputs.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param ch         \IN  Current channel
 *  \param value      \IN  Read value
 *
 *  \return           \c   ERR_LL_ILL_FUNC
 ***********************************************************************************/
static int32 PI7C9_GPIO_Write(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 value
)
{
	int32		i, reg_old, reg_new, error=0,mask=0;

    DBGWRT_1((DBH, "LL - PI7C9_GPIO_Write: ch=%d\n",ch));

	if ( (error = OSS_PciGetConfig(
		llHdl->osHdl, llHdl->pciBus, llHdl->pciDev, llHdl->pciFunc,
		OSS_PCI_ACCESS_32 | GPIO_REG, &reg_old))
	) return error;

	for( i=0; i<GPIO_COUNT; i++ )
		mask |= ((value >> i) & 0x01) << (2+(i*4));		

	reg_new = (reg_old & 0x22) | mask;

	if ( (error = OSS_PciSetConfig(
		llHdl->osHdl, llHdl->pciBus, llHdl->pciDev, llHdl->pciFunc,
		OSS_PCI_ACCESS_32 | GPIO_REG, reg_new))
	) return error;

    DBGWRT_2((DBH, " new=0x%08x\n",
		reg_new));

	return(ERR_SUCCESS);
}

/****************************** PI7C9_GPIO_SetStat *********************************/
/** Set the driver status
 *
 *  The driver supports	\ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl			\IN  Low-level handle
 *  \param code				\IN  \ref getstat_setstat_codes "status code"
 *  \param ch				\IN  Current channel
 *  \param value32_or_64	\IN  Data or pointer to block data structure (M_SG_BLOCK)
 *							for block status codes
 *  \return					\c 0 On success or error code
 ***********************************************************************************/
static int32 PI7C9_GPIO_SetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 value32_or_64
)
{

	int32 error = ERR_SUCCESS;

	int32		value  	= (int32)value32_or_64;	/* 32bit value */
	int32		i, reg_old, reg_new=0, mask=0;

    DBGWRT_1((DBH, "LL - PI7C9_GPIO_SetStat: ch=%d code=0x%04x value=0x%x\n",
			  ch,code,value));

    switch(code) {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            llHdl->dbgLevel = value;
            break;
        /*--------------------------+
        |  set IO ports             |
        +--------------------------*/
        case PI7C9_GPIO_SET_PORTS:
        /*--------------------------+
        |  clear IO ports           |
        +--------------------------*/
        case PI7C9_GPIO_CLR_PORTS:
			DBGWRT_2((DBH, "SetStat PI7C9_GPIO_CLR/SET_PORTS:\n"));
			/* read out current register content */
			if ( (error = OSS_PciGetConfig(
				llHdl->osHdl, llHdl->pciBus, llHdl->pciDev, llHdl->pciFunc,
				OSS_PCI_ACCESS_32 | GPIO_REG, &reg_old))
			) return error;

		    DBGWRT_2((DBH, " reg_old = 0x%08x\n", reg_old));

			/* iterate trough all 8 port bits: */
			/*
			 *	if gpio bits are set, they are ORed into the new register value.
			 *	if bits are to be cleared they must be cleared in old_reg value or they
			 *	remain in new register setting
			 */

			for( i=0; i<GPIO_COUNT; i++ ) {	 
				if( code == PI7C9_GPIO_SET_PORTS ) {	/* set? */
					mask    |= ((value >> i) & 0x01) << (2+(i*4));
				} else {
					reg_old &= ~(((value >> i) & 0x01) << (2+(i*4)));
				}
			}

			DBGWRT_2((DBH, " mask    = 0x%08x\n", mask));

			if( code == PI7C9_GPIO_SET_PORTS ) {	/* set? */
				reg_new = reg_old | mask; /* in case of set the new bits are ORed into old_reg */
			} else { 
				reg_new = reg_old; /* in case of clear the data bits were cleare above, just assign reg_old */
			}

		    DBGWRT_2((DBH, " reg_new = 0x%08x\n", reg_new));

			/* 3. assign new register value to GPIO Reg */			
			if ( (error = OSS_PciSetConfig(
				llHdl->osHdl, llHdl->pciBus, llHdl->pciDev, llHdl->pciFunc,
				OSS_PCI_ACCESS_32 | GPIO_REG, reg_new))
			) return error;



            break;
        /*--------------------------+
        |  port direction           |
        +--------------------------*/
        case PI7C9_GPIO_DIRECTION:
			DBGWRT_2((DBH, "SetStat PI7C9_GPIO_DIRECTION \n"));
			if ( (error = OSS_PciGetConfig(
				llHdl->osHdl, llHdl->pciBus, llHdl->pciDev, llHdl->pciFunc,
				OSS_PCI_ACCESS_32 | GPIO_REG, &reg_old))
			) return error;
			DBGWRT_2((DBH, " reg_old = 0x%08x\n", reg_old));

			/* 1. set each selected port bits direction to 1 (OE=1 = output) */
			for( i=0; i<GPIO_COUNT; i++ ) {
				mask |= ((value >> i) & 0x01) << (1+(i*4));			 
			}
			DBGWRT_2((DBH, " mask    = 0x%08x\n", mask));

			reg_new = reg_old | mask;

			if ( (error = OSS_PciSetConfig(
				llHdl->osHdl, llHdl->pciBus, llHdl->pciDev, llHdl->pciFunc,
				OSS_PCI_ACCESS_32 | GPIO_REG, reg_new))
			) return error;

			DBGWRT_2((DBH, " reg_new = 0x%08x\n", reg_new));
            break;
        /*--------------------------+
        |  unknown                  |
        +--------------------------*/
        default:
            error = ERR_LL_UNK_CODE;
    }

	return(error);
}

/****************************** PI7C9_GPIO_GetStat *********************************/
/** Get the driver status
 *
 *  The driver supports \ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl			\IN  Low-level handle
 *  \param code				\IN  \ref getstat_setstat_codes "status code"
 *  \param ch				\IN  Current channel
 *  \param value32_or_64P	\IN  Pointer to block data structure (M_SG_BLOCK) for
 *  						block status codes
 *  \param value32_or_64P	\OUT Data pointer or pointer to block data structure
 *  						(M_SG_BLOCK) for block status codes
 *
 *  \return					\c 0 On success or error code
 ***********************************************************************************/
static int32 PI7C9_GPIO_GetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 *value32_or_64P
)
{
	int32 error = ERR_SUCCESS;

	int32		*valueP		= (int32*)value32_or_64P;	/* pointer to 32bit value  */
	INT32_OR_64	*value64P	= value32_or_64P;		 	/* stores 32/64bit pointer */
	int32		i, reg, mask=0;

    DBGWRT_1((DBH, "LL - PI7C9_GPIO_GetStat: ch=%d code=0x%04x\n",
			  ch,code));

    switch(code)
    {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            *valueP = llHdl->dbgLevel;
            break;
        /*--------------------------+
        |  number of channels       |
        +--------------------------*/
        case M_LL_CH_NUMBER:
            *valueP = CH_NUMBER;
            break;
        /*--------------------------+
        |  channel direction        |
        +--------------------------*/
        case M_LL_CH_DIR:
            *valueP = M_CH_INOUT;
            break;
        /*--------------------------+
        |  channel length [bits]    |
        +--------------------------*/
        case M_LL_CH_LEN:
            *valueP = GPIO_COUNT;
            break;
        /*--------------------------+
        |  channel type info        |
        +--------------------------*/
        case M_LL_CH_TYP:
            *valueP = M_CH_BINARY;
            break;
        /*--------------------------+
        |  port direction           |
        +--------------------------*/
        case PI7C9_GPIO_DIRECTION:

			if ( (error = OSS_PciGetConfig(
				llHdl->osHdl, llHdl->pciBus, llHdl->pciDev, llHdl->pciFunc,
				OSS_PCI_ACCESS_32 | GPIO_REG, &reg))
			) return error;

 	        for( i=0; i<GPIO_COUNT; i++ )
				mask |= ((reg >> (1+(i*4))) & 0x01) << i;

			DBGWRT_2((DBH, "GetStat PI7C9_GPIO_DIRECTION: valueP = 0x%02x\n",mask));
            *valueP = mask;
            break;
		/*--------------------------+
        |   ident table pointer     |
        |   (treat as non-block!)   |
        +--------------------------*/
        case M_MK_BLK_REV_ID:
           *value64P = (INT32_OR_64)&llHdl->idFuncTbl;
           break;
        /*--------------------------+
        |  unknown                  |
        +--------------------------*/
        default:
            error = ERR_LL_UNK_CODE;
    }

	return(error);
}

/****************************** PI7C9_GPIO_BlockRead *******************************/
/** Read a data block from the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl		\IN  Low-level handle
 *  \param ch			\IN  Current channel
 *  \param buf			\IN  Data buffer
 *  \param size			\IN  Data buffer size
 *  \param nbrRdBytesP	\OUT Number of read bytes
 *
 *  \return				\c ERR_LL_ILL_FUNC
 ***********************************************************************************/
static int32 PI7C9_GPIO_BlockRead(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrRdBytesP
)
{
    DBGWRT_1((DBH, "LL - PI7C9_GPIO_BlockRead: ch=%d, size=%d\n",ch,size));

	/* return number of read bytes */
	*nbrRdBytesP = 0;

	return(ERR_LL_ILL_FUNC);
}

/****************************** PI7C9_GPIO_BlockWrite ******************************/
/** Write a data block from the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl  	   \IN  Low-level handle
 *  \param ch          \IN  Current channel
 *  \param buf         \IN  Data buffer
 *  \param size        \IN  Data buffer size
 *  \param nbrWrBytesP \OUT Number of written bytes
 *
 *  \return            \c ERR_LL_ILL_FUNC
 ***********************************************************************************/
static int32 PI7C9_GPIO_BlockWrite(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrWrBytesP
)
{
    DBGWRT_1((DBH, "LL - PI7C9_GPIO_BlockWrite: ch=%d, size=%d\n",ch,size));

	/* return number of written bytes */
	*nbrWrBytesP = 0;

	return(ERR_LL_ILL_FUNC);
}


/****************************** PI7C9_GPIO_Irq ************************************/
/** Interrupt service routine
 *
 *  The driver supports no interrupts and always returns LL_IRQ_DEV_NOT
 *
 *  \param llHdl  	   \IN  Low-level handle
 *  \return LL_IRQ_DEVICE	IRQ caused by device
 *          LL_IRQ_DEV_NOT  IRQ not caused by device
 *          LL_IRQ_UNKNOWN  Unknown
 ***********************************************************************************/
static int32 PI7C9_GPIO_Irq(
   LL_HANDLE *llHdl
)
{
	return(LL_IRQ_DEV_NOT);
}

/****************************** PI7C9_GPIO_Info ***********************************/
/** Get information about hardware and driver requirements
 *
 *  The following info codes are supported:
 *
 * \code
 *  Code                      Description
 *  ------------------------  -----------------------------
 *  LL_INFO_HW_CHARACTER      Hardware characteristics
 *  LL_INFO_ADDRSPACE_COUNT   Number of required address spaces
 *  LL_INFO_ADDRSPACE         Address space information
 *  LL_INFO_IRQ               Interrupt required
 *  LL_INFO_LOCKMODE          Process lock mode required
 * \endcode
 *
 *  The LL_INFO_HW_CHARACTER code returns all address and
 *  data modes (ORed) which are supported by the hardware
 *  (MDIS_MAxx, MDIS_MDxx).
 *
 *  The LL_INFO_ADDRSPACE_COUNT code returns the number
 *  of address spaces used by the driver.
 *
 *  The LL_INFO_ADDRSPACE code returns information about one
 *  specific address space (MDIS_MAxx, MDIS_MDxx). The returned
 *  data mode represents the widest hardware access used by
 *  the driver.
 *
 *  The LL_INFO_IRQ code returns whether the driver supports an
 *  interrupt routine (TRUE or FALSE).
 *
 *  The LL_INFO_LOCKMODE code returns which process locking
 *  mode the driver needs (LL_LOCK_xxx).
 *
 *  \param infoType	   \IN  Info code
 *  \param ...         \IN  Argument(s)
 *
 *  \return            \c 0 On success or error code
 ***********************************************************************************/
static int32 PI7C9_GPIO_Info(
   int32  infoType,
   ...
)
{
    int32   error = ERR_SUCCESS;
    va_list argptr;

    va_start(argptr, infoType );

    switch(infoType) {
		/*-------------------------------+
        |  hardware characteristics      |
        |  (all addr/data modes ORed)    |
        +-------------------------------*/
        case LL_INFO_HW_CHARACTER:
		{
			u_int32 *addrModeP = va_arg(argptr, u_int32*);
			u_int32 *dataModeP = va_arg(argptr, u_int32*);

			*addrModeP = MDIS_MA08;
			*dataModeP = MDIS_MD08 | MDIS_MD16;
			break;
	    }
		/*-------------------------------+
        |  nr of required address spaces |
        |  (total spaces used)           |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE_COUNT:
		{
			u_int32 *nbrOfAddrSpaceP = va_arg(argptr, u_int32*);

			*nbrOfAddrSpaceP = ADDRSPACE_COUNT;
			break;
	    }
		/*-------------------------------+
        |  address space type            |
        |  (widest used data mode)       |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE:
		{
			u_int32 addrSpaceIndex = va_arg(argptr, u_int32);
			u_int32 *addrModeP = va_arg(argptr, u_int32*);
			u_int32 *dataModeP = va_arg(argptr, u_int32*);
			u_int32 *addrSizeP = va_arg(argptr, u_int32*);

			if (addrSpaceIndex >= ADDRSPACE_COUNT)
				error = ERR_LL_ILL_PARAM;
			else {
				*addrModeP = MDIS_MA_PCICFG;
				*dataModeP = MDIS_MD_NONE;
				*addrSizeP = ADDRSPACE_SIZE;
			}

			break;
	    }
		/*-------------------------------+
        |   interrupt required           |
        +-------------------------------*/
        case LL_INFO_IRQ:
		{
			u_int32 *useIrqP = va_arg(argptr, u_int32*);

			*useIrqP = USE_IRQ;
			break;
	    }
		/*-------------------------------+
        |   process lock mode            |
        +-------------------------------*/
        case LL_INFO_LOCKMODE:
		{
			u_int32 *lockModeP = va_arg(argptr, u_int32*);

			*lockModeP = LL_LOCK_CALL;
			break;
	    }
		/*-------------------------------+
        |   (unknown)                    |
        +-------------------------------*/
        default:
          error = ERR_LL_ILL_PARAM;
    }

    va_end(argptr);
    return(error);
}

/****************************** Ident **********************************************/
/** Return ident string
 *
 *  \return            Pointer to ident string
 ***********************************************************************************/
static char* Ident( void )
{
    return( "PI7C9_GPIO - PI7C9_GPIO low-level driver: $Id: pi7c9_gpio_drv.c,v 1.2 2012/08/02 12:42:54 ts Exp $" );
}

/****************************** Cleanup ********************************************/
/** Close all handles, free memory and return error code
 *
 *	\warning The low-level handle is invalid after this function is called.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param retCode    \IN  Return value
 *
 *  \return           \IN   retCode
 ***********************************************************************************/
static int32 Cleanup(
   LL_HANDLE    *llHdl,
   int32        retCode
)
{
    /*------------------------------+
    |  free resources               |
    +------------------------------*/

    /*------------------------------+
    |  close handles                |
    +------------------------------*/
	/* clean up desc */
	if (llHdl->descHdl)
		DESC_Exit(&llHdl->descHdl);

	/* clean up debug */
	DBGEXIT((&DBH));

    /*------------------------------+
    |  free memory                  |
    +------------------------------*/
    /* free my handle */
    OSS_MemFree(llHdl->osHdl, (int8*)llHdl, llHdl->memAlloc);

    /*------------------------------+
    |  return error code            |
    +------------------------------*/
	return(retCode);
}
