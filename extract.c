//This program will extract all data from the ifstat command and save them
//Current version: 1.1
//History;
//-->1.1: ignores the next line of KB/s
//-->1.0: extracts only name using strtok()


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


#define BUFF_SIZE 300
#define COMM_SIZE 13


struct timeval start,end;


int main()
{
	FILE *ifstatCaller; //This will call ifstat command


	char *buff;//keeping it dynamic
	buff = calloc(BUFF_SIZE,sizeof(char));

	char command[] = "ifstat 0.1 1";

	ifstatCaller = popen(command,"r");//calling the command


	gettimeofday(&start,NULL);

	char* success = fgets(buff,BUFF_SIZE,ifstatCaller);//getting string

	if (success != NULL)
	{
		buff = realloc(buff,strlen(buff)+1);//memory optimization
		printf("String obtained: %s",buff);//outputting the result


		char *name = strtok(buff," ");
		if (name != NULL)
			printf("Name Extracted:%s\n",name);
		name = NULL;
		fgets(buff,BUFF_SIZE,ifstatCaller);//ignores the next line that has KB
		printf("String obtained: %s",buff);//outputting the result


		char *update = fgets(buff,BUFF_SIZE,ifstatCaller);

		if( update != NULL )
			printf("String obtained: %s",buff);//outputting the result


		update = NULL;
		success = NULL;
	}
	else
		printf("String not obtained\n");

	gettimeofday(&end,NULL);


	pclose(ifstatCaller);
	ifstatCaller = NULL;

	free(buff);
	buff = NULL;


{
	int seconds = end.tv_sec - start.tv_sec;
	long time = (seconds*1e6) + end.tv_usec - start.tv_usec;

	printf("\n-----Time elapsed in main execution: %ld microseconds-----\n",time);
}


	return 0;
}
