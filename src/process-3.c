#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <time.h>

size_t ARRAY_SIZE = 16;

unsigned char SUBBOX[256] = {47, 164, 147, 166, 221, 246, 1, 13, 198, 78, 102, 219, 75, 97, 62, 140,
			     84, 69, 107, 99, 185, 220, 179, 61, 187, 0, 92, 112, 8, 33, 15, 119,
			     209, 178, 192, 12, 121, 239, 117, 96, 100, 126, 118, 199, 208, 50, 42, 168,
			     14, 171, 17, 238, 158, 207, 144, 58, 127, 182, 146, 71, 68, 157, 154, 88,
			     248, 105, 131, 235, 98, 170, 22, 160, 181, 4, 254, 70, 202, 225, 67, 205,
			     216, 25, 43, 222, 236, 128, 122, 77, 59, 145, 167, 54, 20, 55, 152, 149,
			     230, 211, 224, 111, 165, 124, 16, 243, 213, 114, 116, 63, 64, 176, 31, 161,
			     9, 229, 95, 247, 193, 18, 134, 79, 133, 173, 82, 51, 57, 136, 6, 49,
			     5, 197, 115, 65, 169, 255, 249, 195, 30, 162, 150, 53, 83, 46, 228, 81,
			     237, 104, 28, 223, 217, 251, 200, 60, 132, 194, 151, 137, 191, 74, 201, 103,
			     29, 80, 113, 101, 250, 172, 234, 180, 73, 141, 204, 27, 241, 188, 153, 155,
			     86, 94, 177, 87, 39, 91, 2, 48, 35, 40, 120, 159, 184, 123, 215, 138,
			     210, 108, 76, 106, 36, 189, 125, 226, 252, 37, 66, 156, 253, 218, 85, 203,
			     110, 10, 244, 45, 34, 242, 72, 93, 52, 135, 44, 245, 3, 32, 196, 163,
			     232, 240, 227, 24, 139, 183, 38, 233, 130, 143, 109, 41, 174, 231, 129, 23,
			     148, 89, 212, 19, 21, 142, 7, 214, 56, 90, 11, 190, 175, 206, 26, 186};

unsigned char get_subbox_val (unsigned char val)
{
  unsigned char sub_box_val = SUBBOX[val];

  return sub_box_val;
}
void writeFile(int process_number, int size, int plain[size]) {

    char file_name[10];				/* for file name */

    int i; 				/* for loop */

    /* the file is opened with the file name created here using the number of incoming process number */
    sprintf(file_name, "process-%d", process_number);
    strcat(file_name, ".txt");
    FILE *output = fopen(file_name, "w");


    for(i = 0; i < size; i++) {

	    fprintf(output, "%d", plain[i]);	/* write matrix element to file */

        if(i!=15)
            fprintf(output, "-");
    }
    fprintf(output,"\n");
    fclose(output);					/* close file */

}



int main(int argc, char * argv[]){

    int * plainBlock =(int*) malloc (16 * sizeof(int));

    int process_no, parent_pid;
    int pid;

    parent_pid = getpid();
    process_no = 3;
    fork();
    if(parent_pid!=getpid())
    {
        process_no=4;
    }


    int pipefd[2];
    pipe(pipefd);

    if(process_no==3){
        int i=1;
        for(;i<17;i++)
        {
            plainBlock[i-1]=atoi(argv[i]);

        }
        int j = 0;
        for(; j< 16; j++ ) {
            unsigned char rand_ind = plainBlock[j] % 256;
            plainBlock[j]=get_subbox_val(rand_ind);
        }
        writeFile(3,16,plainBlock);

        write(pipefd[1],plainBlock,16*sizeof(int));
         close(pipefd[1]);
        close(pipefd[0]);

    int t=1;
        char* c_array[17];
        c_array[0] = malloc(20*sizeof(char));
        strcpy(c_array[0],"./main");

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


