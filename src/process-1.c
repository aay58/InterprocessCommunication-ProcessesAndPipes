#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
void xorOperaton(int size,int plain[size],int key[size])
{
    int i=0;
    for(i=0;i<size;i++)
    {

       plain[i]=plain[i]^key[i];
    }

}
void writeFile(int process_number, int size, int plain[size]) {

    char file_name[10];				/* for file name */

    int i; 				/* for loop */

    /* the file is opened with the file name created here using the number of incoming process number */
    sprintf(file_name, "process-%d", process_number);
    strcat(file_name, ".txt");
    FILE *output = fopen(file_name, "w");


    for(i = 0; i < size; i++) {

	    fprintf(output, "%d", plain[i]);	/* write plain element to file */
        if(i!=15)
            fprintf(output, "-");

    }

    fprintf(output,"\n");
    fclose(output);					/* close file */

}



int main(int argc, char * argv[]){

    int * plainBlock =(int*) malloc (16 * sizeof(int));
    int * keyBlock=(int*) malloc (16 * sizeof(int));
    int process_no, parent_pid;
    int pid;

    parent_pid = getpid();
    process_no = 1;


    fork();
    if(parent_pid!=getpid())
    {
        process_no=2;

    }

    int pipefd[2];
    pipe(pipefd);

    if(process_no==1){
        int i=1;
        for(;i<17;i++)
        {
            plainBlock[i-1]=atoi(argv[i]);

        }
        for(i=17;i<33;i++)
        {
            keyBlock[i-17]=atoi(argv[i]);
        }

        xorOperaton(16,plainBlock,keyBlock);


        writeFile(1,16,plainBlock);

        write(pipefd[1],plainBlock,16*sizeof(int));
        close(pipefd[1]);
        close(pipefd[0]);
        int t=1;
        char* c_array[17];
        c_array[0] = malloc(20*sizeof(char));
        strcpy(c_array[0],"./process-2");
        for (; t < 17; t++)
        {
            char c[sizeof(int)];

            snprintf(c, sizeof(int), "%d", plainBlock[t-1]); /*copy those 4bytes*/
          /* allocate enough space on char* array to store this result*/
            c_array[t] = malloc(sizeof(c));
            strcpy(c_array[t], c); /* copy to the array of results*/
        }
        execvp(c_array[0], c_array);

    }

    return 0;

}


