/********Software Analysis - FY2013*************/
/*
* File Name: ow_memcpy.c
* Defect Classification
* ---------------------
* Defect Type: Dynamic memory defects
* Defect Sub-type: Memory copy at overlapping areas
*
*/

#include "HeaderFile.h"

/*
 * Types of defects: copy of the overlapped area
 * Complexity: loop in a function
 */
void ow_memcpy_001 ()
{
	int buf[5] = {1, 2, 3, 4, 5};
	int i;

#ifdef __TRUSTINSOFT_BUGFIX__
	/*
	 * FAULTY TEST:
	 * There is no "memory copy at overlapping areas" if we copy cells in an
	 * array one by one. This program's behavior is valid and well-defined.
	 * We need to *actually* call the memcpy() function for this to be
	 * Undefined Behavior, as per https://cigix.me/c17#7.24.2.1:
	 * 
	 * 		The memcpy function copies n characters from the object pointed to
	 * 		by s2 into the object pointed to by s1. If copying takes place
	 * 		between objects that overlap, the behavior is undefined.
	 * 
	 * This is a "memcpy()" call replacing the subsequent for-loop: */
	int *s1 = &buf[0];
	int *s2 = &buf[2];
	size_t sz = 3 * sizeof(int);
	memcpy(s1, s2, sz);
#else
	for (i = 0; i < 3; i ++)
	{
		buf[i] = buf[i+2];/*Tool should detect this line as error*/ /*ERROR:copy of the overlapped area*/
	}
#endif
}

/*
 * Types of defects: copy of the overlapped area
 * Complexity: when calling a function (equivalent to memcpy)
 */
void ow_memcpy_002_func_001 (void *dst, const void *src, int size)
{
	unsigned char *p;
	unsigned char *q;
	int i;
	p = (unsigned char *)src;
	q = (unsigned char *)dst;
#ifdef __TRUSTINSOFT_BUGFIX__
	/* 
	 * FAULTY TEST:
	 * See previous case.
	 */
	memcpy(dst, src, size);
#else
	for (i = 0; i < size; i ++)
	{
		*q = *p;/*Tool should detect this line as error*/ /*ERROR:copy of the overlapped area*/
		p ++;
		q ++;
	}
#endif
}

void ow_memcpy_002 ()
{
	int buf[5] = {1, 2, 3, 4, 5};
	ow_memcpy_002_func_001(&buf[2], &buf[0], (3 * sizeof(int)));
}

/*
 * Types of defects: copy of the overlapped area
 * Complexity: volatile
 */
extern volatile int vflag;
void ow_memcpy_main ()
{
	if (vflag ==1 || vflag ==888)
	{
		ow_memcpy_001();
	}

	if (vflag ==2 || vflag ==888)
	{
		ow_memcpy_002();
	}
}
