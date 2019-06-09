#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define maxSize 256
#ifndef _HELPERS 
#define _HELPERS 

/*-----------------ZIP-------------------------*/
extern int convertDecimalToOctal(unsigned short decimalNumber);
extern char * itoa(int number);
extern char * getPermission(char file_name[]);
extern off_t findSize(char file_name[]);
extern void insertRecord(char record[],int fd);
extern short checkAscii(char * fileName);
/*-----------------/ZIP-------------------------*/

/*-----------------UNZIP-------------------------*/
extern void sauFileCheck(int * fileNameSize,char ** params);
extern void parameterCheck(int parameterCount,char ** params);

/*-----------------/UNZIP-------------------------*/

#endif
