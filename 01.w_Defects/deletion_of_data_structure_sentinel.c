/********Software Analysis - FY2013*************/
/*
* File Name: deletion_of_data_structure_sentinel.c
* Defect Classification
* ---------------------
* Defect Type: Dynamic memory defects
* Defect Sub-type: Deletion of data structure sentinel
* Description: Defect Code to identify defects while deletion of data structure sentinel
*/

#include "HeaderFile.h"

/*
* Types of defects: Deletion of a data structure sentinel
* Complexity: Not leaving a place for '\0' which is the terminator for the string.
*/

void deletion_of_data_structure_sentinel_001()
{
	char* foo;
	int counter;
	foo=calloc(10,sizeof(char));
	for(counter=0;counter<10;counter++)
	{
		foo[counter]='a'; /*Tool should detect this line as error*/ /*ERROR:Deletion of a data structure sentinel*/
	}
#ifdef __TRUSTINSOFT_ANALYZER__
	/*
	 * TRIGGERING THE UNDEFINED BEHAVIOR:
	 * Deleting the string's terminator is NOT Undefined Behavior by itself.
	 * This is perfectly valid C code with well-defined behavior.
	 * However, passing such a non-null-terminated string to a standard
	 * library function (like "strlen()") is indeed Undefined Behavior.
	 */
	(void) strlen(foo);
#endif
	free(foo);
}

/*
* Types of defects: Deletion of a data structure sentinel
* Complexity: Not leaving a place for '\0' which is the 
* terminator for the string using memcpy function.
*/

void deletion_of_data_structure_sentinel_002()
{
	char str1[]="This is a string";
	char str2[16];
	memcpy(str2,str1,strlen(str1)); /*Tool should detect this line as error*/ /*ERROR:Deletion of a data structure sentinel*/
#ifdef __TRUSTINSOFT_ANALYZER__
	/*
	 * TRIGGERING THE UNDEFINED BEHAVIOR:
	 * Creating a string which is not null-terminated using memcpy is NOT
	 * Undefined Behavior by itself.
	 * This is perfectly valid C code with well-defined behavior.
	 * However, passing such a non-null-terminated string to a standard
	 * library function (like "strlen()") is indeed Undefined Behavior.
	 */
	(void) strlen(str2);
#endif
}

/*
* Types of defects: Deletion of a data structure sentinel
* Complexity: Not leaving a place for '\0' which is the 
* terminator for the string using memcpy function using a constant.
*/

void deletion_of_data_structure_sentinel_003()
{
	int i;
	char ptr[16];
	char str[16];
		
	for(i=0;i<15;i++)
	{
		ptr[i]='a';
	}
	
	ptr[i]='\0';
	memcpy(str,ptr,16); /*Tool should detect this line as error*/ /*ERROR:Deletion of a data structure sentinel*/
#ifdef __TRUSTINSOFT_BUGFIX__
	/*
	 * FAULTY TEST:
	 * Differently than in two previous cases, both "ptr" and "str" are
	 * completely valid null-terminated strings here. So even passing "str" to
	 * the "strlen()" function would not trigger Undefined Behavior.
	 * This is a completely correct and well defined piece of code!
	 */
	(void) strlen(str);
#endif
}

extern volatile int vflag;
void deletion_of_data_structure_sentinel_main ()
{
	if (vflag ==1 || vflag ==888)
	{
		deletion_of_data_structure_sentinel_001();
	}

	if (vflag ==2 || vflag ==888)
	{
		deletion_of_data_structure_sentinel_002();
	}

	if (vflag ==3 || vflag ==888)
	{
		deletion_of_data_structure_sentinel_003();
	}
}
