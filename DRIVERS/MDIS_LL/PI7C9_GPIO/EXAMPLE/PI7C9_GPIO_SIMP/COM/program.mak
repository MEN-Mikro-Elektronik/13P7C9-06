#***************************  M a k e f i l e  *******************************
#
#         Author: km
#          $Date: 2012/07/17 13:43:31 $
#      $Revision: 1.1 $
#
#    Description: Makefile definitions for the PI7C9 GPIO example tool
#
#---------------------------------[ History ]--------------------------------
#
#   $Log: program.mak,v $
#   Revision 1.1  2012/07/17 13:43:31  ts
#   Initial Revision
#
#   Revision 1.1  2006/08/02 08:31:53  DPfeuffer
#   Initial Revision
#
#----------------------------------------------------------------------------
#   (c) Copyright 2012 by MEN mikro elektronik GmbH, Nuernberg, Germany
#*****************************************************************************

MAK_NAME=pi7c9_gpio_simp

MAK_LIBS=$(LIB_PREFIX)$(MEN_LIB_DIR)/mdis_api$(LIB_SUFFIX)	\
		 $(LIB_PREFIX)$(MEN_LIB_DIR)/usr_oss$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/usr_utl$(LIB_SUFFIX)	\

MAK_INCL=$(MEN_INC_DIR)/men_typs.h	\
         $(MEN_INC_DIR)/usr_utl.h	\
         $(MEN_INC_DIR)/mdis_api.h	\
         $(MEN_INC_DIR)/usr_oss.h	\
         $(MEN_INC_DIR)/pi7c9_gpio_drv.h	\

MAK_INP1=pi7c9_gpio_simp$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)
