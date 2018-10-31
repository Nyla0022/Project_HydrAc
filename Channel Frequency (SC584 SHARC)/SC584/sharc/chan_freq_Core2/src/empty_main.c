/*****************************************************************************
 * empty_main.c
 *****************************************************************************/

/* Managed drivers and/or services include */
#include "../system/adi_initialize.h"
int main(int argc, char *argv[])
{
	/* Initialize managed drivers and/or services */
	adi_initComponents();

	/* This core is not used in this example. */
	/* If you need to use Core2, create a new */
	/* project which will then contain the    */
	/* necessary initialization code.         */
	return 0;
}
