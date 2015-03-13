#***************************  M a k e f i l e  *******************************
#
#         Author: km
#          $Date: 2012/07/17 17:14:55 $
#      $Revision: 1.1 $
#
#    Description: Makefile definitions for the PI7C9_GPIO driver, for swapped
#                 platforms. 
#
#---------------------------------[ History ]---------------------------------
#
#   $Log: driver_sw.mak,v $
#   Revision 1.1  2012/07/17 17:14:55  ts
#   Initial Revision
#
#
#-----------------------------------------------------------------------------
#   (c) Copyright 2012 by MEN mikro elektronik GmbH, Nuremberg, Germany
#*****************************************************************************

MAK_NAME=pi7c9_gpio_sw

MAK_SWITCH=$(SW_PREFIX)MAC_MEM_MAPPED \
			$(SW_PREFIX)MAC_BYTESWAP \

MAK_LIBS=$(LIB_PREFIX)$(MEN_LIB_DIR)/desc$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/oss$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/dbg$(LIB_SUFFIX)	\

MAK_INCL=$(MEN_INC_DIR)/pi7c9_gpio.h	\
         $(MEN_INC_DIR)/men_typs.h	\
         $(MEN_INC_DIR)/oss.h		\
         $(MEN_INC_DIR)/mdis_err.h	\
         $(MEN_INC_DIR)/maccess.h	\
         $(MEN_INC_DIR)/desc.h		\
         $(MEN_INC_DIR)/mdis_api.h	\
         $(MEN_INC_DIR)/mdis_com.h	\
         $(MEN_INC_DIR)/ll_defs.h	\
         $(MEN_INC_DIR)/ll_entry.h	\
         $(MEN_INC_DIR)/dbg.h		\

MAK_INP1=pi7c9_gpio_drv$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)
