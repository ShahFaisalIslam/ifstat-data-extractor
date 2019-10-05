//This program will extract all data from the ifstat command and save them
//Current version: 1.7.2
//History;
//-->1.7.2: Converted buffer into a character array
//-->1.7.1: Removed the reallocation of buffer, as the next line could be greater in size. in other words
//-->1.7:   user can input delay of their choice
//-->1.6.1: renamed net_name variable to dev_name in order to reflect that it is name of devices
//-->1.6:   saves the data in respective files
//-->1.5:   stores name too
//-->1.4:   extracts the numbers and stores them in float array
//-->1.3:   extracts the numbers and stores them in two string arrays
//-->1.2:   fetches first line of output
//-->1.1:   ignores the next line of KB/s
//-->1.0:   extracts only name using strtok()



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>



#define BUFF_SIZE 300
#define COMM_SIZE 17
#define INOUT 2
#define MAX_NUM_SIZE 8 //xxxx.xx\0
#define NAME_SIZE 31
#define DELAY_LENGTH 6



struct timeval start,end;



int main(int argc, char* argv[])
{
//If user inputs custom delay, it gets shown here
	char delay[DELAY_LENGTH] = "0.1";

	if (argv[1] != NULL)
		strcpy(delay,argv[1]);


//
gettimeofday(&start,NULL);
	FILE *ifstatCaller; //This will call ifstat command

	char net_name[NAME_SIZE];

	char inout[INOUT][MAX_NUM_SIZE];
	double inout_n[INOUT];

	char buff[BUFF_SIZE];

//creating the command for ifstat
	char command[COMM_SIZE];
	strcpy(command,"ifstat ");
	strcat(command,delay);
	strcat(command," 1"); //ifstat <delay> 1


//calling the command to ifstat
	ifstatCaller = popen(command,"r");//calling the command


//extracting the first line
	char* success = fgets(buff,BUFF_SIZE,ifstatCaller);//getting string

	if (success != NULL)//if we are successful in doing so, it means the rest of the operations should be done now
	{
		//printf("String obtained: %s",buff);//outputting the result



		char *name = strtok(buff," ");
		if (name != NULL)
			{
				strcpy(net_name,name);
				//printf("Name Extracted:%s\n",net_name);
			}
		name = NULL;
		fgets(buff,BUFF_SIZE,ifstatCaller);//ignores the next line that has KB
		//printf("String obtained: %s",buff);//outputting the result


//extracting the in/out update. May turn into multiple runs in future
		char *update = fgets(buff,BUFF_SIZE,ifstatCaller);

		if( update != NULL )
		{
			update = strtok(update," ");
			strcpy(inout[0],update);
			inout_n[0] = strtod(update,NULL);

			update = strtok(NULL," ");
			strcpy(inout[1],update);
//optimization: removal of newline from the second ifstat
			char *newline = strrchr(inout[1],'\n');
			if (newline != NULL)
				memcpy(newline,"\0",1);
			newline = NULL;
//end of optimization

			inout_n[1] = strtod(update,NULL);

			//printf("Update 1: %.2f %.2f\n",inout_n[0],inout_n[1]);//outputting the result
		}

		update = NULL;
		success = NULL;


//creating filenames
		char *in_filename;
		char *out_filename;

		in_filename=calloc(strlen(net_name)+strlen("_in"),sizeof(char));
		out_filename=calloc(strlen(net_name)+strlen("_out"),sizeof(char));

		strcpy(in_filename,net_name);
		strcat(in_filename,"_in");

		strcpy(out_filename,net_name);
		strcat(out_filename,"_out");


//accessing and storing data in files
		FILE *in_file;
		FILE *out_file;

		in_file=fopen(in_filename,"w");
		out_file=fopen(out_filename,"w");

		int success_in =fputs(inout[0],in_file);
		int success_out =fputs(inout[1],out_file);

		if (success_in == 0)
			printf("Failed to write \"in\"\n");
		if (success_out == 0)
			printf("Failed to write \"out\"\n");


//closing files
		free(in_filename);
		in_filename=NULL;

		free(out_filename);
		out_filename=NULL;


		fclose(in_file);
		in_file=NULL;

		fclose(out_file);
		out_file=NULL;

	}
	else//if command does not work
	{
		printf("Unable to execute the command\n");
		return 0;
	}


//closing the command
	pclose(ifstatCaller);
	ifstatCaller = NULL;

gettimeofday(&end,NULL);



{//generating statistics
	int seconds = end.tv_sec - start.tv_sec;
	long time = (seconds*1e6) + end.tv_usec - start.tv_usec;

	printf("-----Time elapsed in main execution: %ld microseconds-----\n",time);
}



	return 0;
}
