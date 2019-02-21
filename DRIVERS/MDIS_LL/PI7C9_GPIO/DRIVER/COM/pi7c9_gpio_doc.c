/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  pi7c9_gpio_doc.c
 *
 *      \author  km
 *        $Date: 2012/07/17 17:52:06 $
 *    $Revision: 1.2 $
 *
 *      \brief   User documentation for PI7C9_GPIO driver
 *
 *     Required: -
 *
 *     \switches -
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

/*! \mainpage
    This is the documentation of the PI7C9_GPIO low-level driver that supports
    the access of the Pericom PCI Express to USB SWIDGE PI7C9X442SL GPIOs.

    The driver supports the following HW features:\n
	- 8xGPIO

    \n
    \section Variants Variant
	The driver has only one Variant.

    Notes:\n
	- The standard driver supports no interrupts.
		
    \n \section FuncDesc Functional Description
	The driver accesses the 8 GPIOs by using the GPIO register at PCI config space address 0xd8

    \n \subsection General General
	The driver accesses the 8 GPIO pins on the Pericom bridge through the PCI config space Register 0x8d.

    \n \subsection channels Logical channels
    The driver provides only one logical channel\n

    \n \section descriptor_entries Descriptor Entries
    
    The low-level driver initialization routine decodes only the general
	descriptor entries ("keys").

	The other driver functionality is supported by the Getstat/Setstat
	codes.
	See \ref getstat_setstat_codes "section about PI7C9_GPIO Getstat/Setstat codes".

    \n \section programs Overview of provided programs

    \subsection pi7c9_gpio_simp  
	     Simple example for using the driver on a gpio pin base.

*/

/** \example pi7c9_gpio_simp.c */


/*! \page dummy
  \menimages
*/

