/****************************************************************************/
/*!
 *         \file pi7c9_gpio_simp
 *       \author km
 *        $Date: 2014/07/17 17:20:12 $
 *    $Revision: 1.5 $
 *
 *       \brief  Tool to access the PI7C9X442SL (8-bit) I/Os
 *
 *     Required: libraries: mdis_api, usr_oss, usr_utl
 *     \switches (none)
 */
 /*
 *---------------------------------------------------------------------------
 * (c) Copyright 2012 by MEN mikro elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <MEN/men_typs.h>
#include <MEN/mdis_api.h>
#include <MEN/usr_oss.h>
#include <MEN/usr_utl.h>
#include <MEN/pi7c9_gpio_drv.h>

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void usage(void);
static void PrintError(char *info);

/****************************** usage **********************************************/
/**  Print program usage
 ***********************************************************************************/
static void usage(void)
{
	printf("\n");
	printf("Usage:    pi7c9_gpio_simp [<opts>] <device> [<opts>]\n");
	printf("Function: Tool to access the PI7C9X442SL (8-bit) I/Os\n");
	printf("Options:\n");
	printf("    device     device name\n");
	printf("    -p=<port>  i/o port 0..7, or all if not set, or bitwise combination\n");
	printf("    -g         get state of input port(s)\n");
	printf("    -s=0/1     set output port(s) low/high\n");			
	printf("    -t         toggle all output ports in turn\n");
	printf("    -G         get state of input port(s) in a loop\n");
	printf("    -T	       toggle output port(s) in a loop\n");			
	printf("    -h         hold path open until keypress\n");
	printf("\n");
	printf("(c) 2012 by MEN mikro elektronik GmbH\n\n");
}

/***********************************************************************************/
/** Program main function
 *
 *  \param argc       \IN  argument counter
 *  \param argv       \IN  argument vector
 *
 *  \return	          success (0) or error (1)
 ***********************************************************************************/
int main(int argc, char *argv[])
{
	int32	n; /*, err, ret=1;*/
	char	*device, *str, *portStr, *errstr, buf[40];
	int32	port, get, set, toggleRot, getLoop, toggleLoop, hold;
	int32	portBit, dir, val;
	MDIS_PATH path;

	/*----------------------+
    |  check arguments      |
    +----------------------*/
	if( (errstr = UTL_ILLIOPT("p=gs=tGTh?", buf)) ){
		printf("*** %s\n", errstr);
		return(1);
	}

	if( UTL_TSTOPT("?") ){
		usage();
		return(1);
	}

	if( argc < 3 ){
		usage();
		return(1);
	}
	
	/*----------------------+
    |  get arguments        |
    +----------------------*/
	for (device=NULL, n=1; n<argc; n++)
		if (*argv[n] != '-') {
			device = argv[n];
			break;
		}

	if (!device) {
		usage();
		return(1);
	}


    /*	port = ((portStr = UTL_TSTOPT("p=")) ? atoi(portStr) : -1);  */

	if ((portStr = UTL_TSTOPT("p="))) {
	    sscanf( portStr, "%x", &port );
		portBit = port & 0xff;
		printf("port value passed: 0x%08x -> portBit=0x%02x\n", port, portBit);
	} else {
		port 	= -1;
		portStr = "0..7";
		portBit = 0xff;
 	}



	get        = (UTL_TSTOPT("g") ? 1 : 0);
	set        = ((str = UTL_TSTOPT("s=")) ? atoi(str) : -1);
	toggleRot  = (UTL_TSTOPT("t") ? 1 : 0);
	getLoop    = (UTL_TSTOPT("G") ? 1 : 0);
	toggleLoop = (UTL_TSTOPT("T") ? 1 : 0);
	hold       = (UTL_TSTOPT("h") ? 1 : 0);

	/*--------------------+
    |  open path          |
    +--------------------*/
	if ((path = M_open(device)) < 0) {
		PrintError("open");
		return(1);
	}

	/*----------------------+
	|  set port direction   |
	+----------------------*/
	if ((M_getstat(path, PI7C9_GPIO_DIRECTION, &dir)) < 0) {
		PrintError("getstat PI7C9_GPIO_DIRECTION");
		goto abort;
	}

	/* input? */
	if( get || getLoop ){
		dir &= ~portBit;
	}
	/* output */
	else {
		dir |= portBit;
	}

	if ((M_setstat(path, PI7C9_GPIO_DIRECTION, dir)) < 0) {
		PrintError("setstat PI7C9_GPIO_DIRECTION");
		goto abort;
	}

	/*----------------------+
	|  get input            |
	+----------------------*/
	if( get || getLoop ){

		/* repeat until keypress */
		do {
			if ((M_read(path, &val ))  < 0 ){
				PrintError("read");
				goto abort;
			}

			if( port == -1 ){
				printf("port : 7  6  5  4  3  2  1  0\n");
				printf("state: ");
				for( n=7; n>=0; n-- )
					printf("%d  ", (val>>n)&1 );
				printf("\n\n");
			}
			else{
				printf("input state port #%d: %d (read-value=0x%x)\n",
					port, (val & portBit) ? 1 : 0, val);
			}

			if( getLoop )
				UOS_Delay(2000);

		} while( getLoop && (UOS_KeyPressed() == -1) );
	}

	/*----------------------+
	|  set output           |
	+----------------------*/
	if( set != -1 ){
		/*
		 *	for each selected portbit to set/clear: 
		 * 	1. set its direction to output
		 *	2. set the data 
		 */
		for( n=0; n<8; n++ ) {
			if (portBit & (1<<n))
				printf("set output state port #%d: %d\n", n, set);
        }

		if ((M_setstat(path, set ? PI7C9_GPIO_SET_PORTS : PI7C9_GPIO_CLR_PORTS, portBit)) < 0) {
			PrintError("setstat PI7C9_GPIO_SET_PORTS/PI7C9_GPIO_CLR_PORTS");
			goto abort;
		}
	}

	/*----------------------+
	|  toggle output        |
	+----------------------*/
	if( toggleRot ){

		printf("toggle output state port 0..7 in turn\n");

		for( n=0; n<8; n++ ){

			if ((M_write(path, 1<<n ))  < 0 ){
				PrintError("write");
				goto abort;
			}

			if( n<8 )
				UOS_Delay(1000);
		}
	}

	if( toggleLoop ){

		int32 out = 0;

		/* repeat until keypress */
		do {
			out = !out;

			printf("set output state port #%s: %d\n",
				portStr, out);

			if ((M_setstat(path, out ? PI7C9_GPIO_SET_PORTS : PI7C9_GPIO_CLR_PORTS, portBit)) < 0) {
				PrintError("setstat PI7C9_GPIO_SET_PORTS/PI7C9_GPIO_CLR_PORTS");
				goto abort;
			}

			UOS_Delay(2000);

		} while( UOS_KeyPressed() == -1 );		
	}

	/* hold path open until keypress */
	if( hold ){
		printf("holding path open until keypress\n");
		UOS_KeyWait();
	}

	/*--------------------+
    |  cleanup            |
    +--------------------*/
	abort:
	if (M_close(path) < 0)
		PrintError("close");

	return(0);
}

/****************************** PrintError *****************************************/
/** Print MDIS error message
 *
 *  \param info       \IN  info string
 ***********************************************************************************/
static void PrintError(char *info)
{
	printf("*** can't %s: %s\n", info, M_errstring(UOS_ErrnoGet()));
}



