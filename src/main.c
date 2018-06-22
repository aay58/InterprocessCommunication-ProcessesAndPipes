#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void writeFile( int plain[16]) {

    char file_name[10];				/* for file name */

    int i; 				/* for loop */

    /* the file is opened with the file name created here using the number of incoming process number */
    sprintf(file_name, "cipher");
    strcat(file_name, ".txt");
    FILE *output = fopen(file_name, "w");


    for(i = 0; i < 16; i++) {

	    fprintf(output, "%d", plain[i]);	/* write matrix element to file */

        if(i!=15)
            fprintf(output, "-");
    }
    fprintf(output,"\n");
    fclose(output);					/* close file */

}

int main(int argc, char * argv[])
{
    int i=1;

    int plain_pfd[2];
    int read_plain, write_plain;		/* for matrix pipe two tips */
    int * keyBlock=(int*) malloc (16 * sizeof(int));
    int * plainBlock = (int*) malloc (16 * sizeof(int));

    int status;
    int process_no, parent_pid;
    int pid;

    parent_pid = getpid();
    process_no = 0;


    fork();
    if(parent_pid!=getpid())
    {
        process_no=1;

    }

    int count=0;
    char *word;

    FILE *in = fopen("plain.txt", "r");	/* open for reading */
    FILE *key = fopen("key.txt", "r");	/* open for reading */

    const size_t line_size = 300;
    char* line = malloc(line_size);

    char* key_array[16];
    while (fgets(line, line_size, key) != NULL)  {

                word = strtok(line, "-");		/* split line by comma */

                while(word != NULL) {

            /* the words are converted to integer the keyBlock is assigned to the required position */
                keyBlock[count]=atoi(word);
                word = strtok(NULL, "-");		/* deleted words */
                count++;

           }
    }
    fclose(key);

    count=0;

    if(argc<2){
        if(process_no==0)
        {
            pipe(plain_pfd);

            read_plain = plain_pfd[0];
            write_plain = plain_pfd[1];

            while (fgets(line, line_size, in) != NULL)  {

                if(count<16){
                    word = strtok(line, "-");		/* split line by - */

                    while(word != NULL) {

                /* the words are converted to integer and then the plainBlock is assigned to the required position */
                        plainBlock[count]=atoi(word);

                        word = strtok(NULL, "-");
                        count++;
                    }
               }
               if(count==16)
               {
                    write(write_plain, &plainBlock, sizeof(unsigned long) );
                    int t=1;
                    char* c_array[34];
                    c_array[0] = malloc(20*sizeof(char));
                     strcpy(c_array[0],"./process-1");
                     for (; t < 17; t++)
                    {
                    /* allocate a big enough char to store an int (which is 4bytes, depending on your platform)*/
                        char c[sizeof(int)];

                    /*copy those 4bytes*/
                        snprintf(c, sizeof(int), "%d", plainBlock[t-1]); /*copy those 4bytes*/

                    /* allocate enough space on char* array to store this result*/
                        c_array[t] = malloc(sizeof(c));
                        strcpy(c_array[t], c); /* copy to the array of results*/
                    }
                    for (t=17; t < 33; t++)
                    {
                        char c[sizeof(int)];

                    /* copy int to char*/
                        snprintf(c, sizeof(int), "%d", keyBlock[t-17]); /*copy those 4bytes*/

           /* allocate enough space on char* array to store this result*/
                            c_array[t] = malloc(sizeof(c));
                        strcpy(c_array[t], c); /* copy to the array of results*/
                    }


                close(read_plain);
                execvp(c_array[0], c_array);
               }
               count=0;


            }
        }
    }
    else
        {
            int i=0;
            for(;i<16;i++)
            {
                plainBlock[i]=atoi(argv[i+1]);
            }
            writeFile(plainBlock);
        }




    return 0;
}
