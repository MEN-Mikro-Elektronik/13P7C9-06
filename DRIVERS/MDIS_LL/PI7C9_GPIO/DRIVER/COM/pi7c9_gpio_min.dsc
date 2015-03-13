#************************** MDIS4 device descriptor *************************
#
#        Author: km
#         $Date: 2012/07/17 13:41:03 $
#     $Revision: 1.1 $
#
#   Description: Metadescriptor for PI7C9_GPIO
#
#****************************************************************************

pi7c9_gpio_1  {
	#------------------------------------------------------------------------
	#	general parameters (don't modify)
	#------------------------------------------------------------------------
    DESC_TYPE        = U_INT32  1           # descriptor type (1=device)
    HW_TYPE          = STRING   PI7C9_GPIO  # hardware name of device

	#------------------------------------------------------------------------
	#	reference to base board
	#------------------------------------------------------------------------
    BOARD_NAME       = STRING   PCI_1       # device name of baseboard
    DEVICE_SLOT      = U_INT32  0           # used slot on baseboard (0..n)

	#------------------------------------------------------------------------
	#	device parameters
	#------------------------------------------------------------------------
}
