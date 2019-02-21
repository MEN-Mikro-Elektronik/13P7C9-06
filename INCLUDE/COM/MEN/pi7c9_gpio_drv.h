/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  pi7c9_gpio_drv.h
 *
 *      \author  km
 *        $Date: 2012/07/17 13:46:32 $
 *    $Revision: 3.1 $
 *
 *       \brief  Header file for PI7C9_GPIO driver containing
 *               PI7C9_GPIO specific status codes and
 *               PI7C9_GPIO function prototypes
 *
 *    \switches  _ONE_NAMESPACE_PER_DRIVER_
 *               _LL_DRV_
 */
 /*
 *---------------------------------------------------------------------------
 * Copyright (c) 2012-2019, MEN Mikro Elektronik GmbH
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

#ifndef _PI7C9_GPIO_DRV_H
#define _PI7C9_GPIO_DRV_H

#ifdef __cplusplus
      extern "C" {
#endif

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/** \name PI7C9_GPIO specific Getstat/Setstat standard codes
 *  \anchor getstat_setstat_codes
 */
/**@{*/			
#define PI7C9_GPIO_SET_PORTS 	M_DEV_OF+0x00   /**<  S: Set GPIOs */
#define PI7C9_GPIO_CLR_PORTS	M_DEV_OF+0x01   /**<  S: Clear GPIOs */
#define PI7C9_GPIO_DIRECTION	M_DEV_OF+0x02   /**<G,S: Get/set GPIOs direction */

/**@}*/

#ifndef  PI7C9_GPIO_VARIANT
# define PI7C9_GPIO_VARIANT PI7C9_GPIO
#endif

# define _PI7C9_GPIO_GLOBNAME(var,name) var##_##name
#ifndef _ONE_NAMESPACE_PER_DRIVER_
# define PI7C9_GPIO_GLOBNAME(var,name) _PI7C9_GPIO_GLOBNAME(var,name)
#else
# define PI7C9_GPIO_GLOBNAME(var,name) _PI7C9_GPIO_GLOBNAME(PI7C9_GPIO,name)
#endif

#define __PI7C9_GPIO_GetEntry    PI7C9_GPIO_GLOBNAME(PI7C9_GPIO_VARIANT,GetEntry)

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
#ifdef _LL_DRV_
#ifndef _ONE_NAMESPACE_PER_DRIVER_
	extern void __PI7C9_GPIO_GetEntry(LL_ENTRY* drvP);
#endif
#endif /* _LL_DRV_ */

/*-----------------------------------------+
|  BACKWARD COMPATIBILITY TO MDIS4         |
+-----------------------------------------*/
#ifndef U_INT32_OR_64
 /* we have an MDIS4 men_types.h and mdis_api.h included */
 /* only 32bit compatibility needed!                     */
 #define INT32_OR_64  int32
 #define U_INT32_OR_64 u_int32
 typedef INT32_OR_64  MDIS_PATH;
#endif /* U_INT32_OR_64 */


#ifdef __cplusplus
      }
#endif

#endif /* _PI7C9_GPIO_DRV_H */
