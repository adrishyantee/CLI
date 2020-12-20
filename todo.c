#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>

#define MAXCHAR 100

// HELP handler
void help() {
    printf("%s\n", "Usage :-");
    printf("%s\n", "$ ./todo add \"todo item\"  # Add a new todo");
    printf("%s\n", "$ ./todo ls               # Show remaining todos");
    printf("%s\n", "$ ./todo del NUMBER       # Delete a todo");
    printf("%s\n", "$ ./todo done NUMBER      # Complete a todo");
    printf("%s\n", "$ ./todo help             # Show usage");
    printf("%s\n", "$ ./todo report           # Statistics");
}

// Add Handler
void add(char *item) {
    // Log the success message
    printf("Added todo: \"%s\"",item);

    // Concatenate a new line
    strcat(item,"\n");

    // Pointer of file type initialized
    FILE *pfile = NULL;

    // Opening the file todo.txt
    // Replace todo.txt with the full path of your file
    char *filename="todo.txt";
    pfile=fopen(filename,"a");

    // Setting the position to the beginning of the file to write
    fseek(pfile,0,SEEK_SET);

    // Addint the item to the file
    fputs(item,pfile);

    // Closing the file
    fclose(pfile);

    pfile=NULL;
}

/* List ToDo Handler
    Current set for three todo, for a generalized once:
    1. The number of ToDos are to be found out
    2. An Array for storing TODOs are to be created with the above found length
    3. The while loop should use the count as index to store the equivalent todo in the ToDo Array.
*/

void ls(){
    int count=0,i=0,j=0;
    char c;
    FILE *pfile = NULL;
    char *filename="todo.txt";
    pfile=fopen(filename,"r");

    if (pfile == NULL) {
        printf("There are no pending todos!\n");
        return;
    }

    char str[20][MAXCHAR];

    for (c = getc(pfile); c != EOF; c = getc(pfile)) 
     {
        if (c == '\n')
        {
            str[i][j] = '\0';
            count = count + 1; 
            i++;
            j=0;
        }
        else 
        {
            str[i][j++]=c;
        }
     }

    for (i=count-1;i>=0;i--)
    {
        printf("[%d] %s\n",i+1, str[i]);
    }  

    fclose(pfile);
    pfile=NULL; 
}

// Delete ToDo Handler
void del(char *n)
{
    FILE *pfile = NULL;
    char *filename="todo.txt";
    FILE *fileptr2;
    char ch;
    int delete_line, temp = 1,count=0;
    delete_line=(int)(n[0]-'0');

    pfile = fopen(filename, "r");
    if (pfile == NULL || delete_line <= 0) {
        printf("Error: todo #%d does not exist. Nothing deleted.",delete_line);
        return;
    }

    fseek(pfile,0,SEEK_SET);
    for (ch = getc(pfile); ch != EOF; ch = getc(pfile)) 
    {
        if (ch== '\n')
            count = count + 1; 
    }

    if (delete_line <= 0 || delete_line > count )
        printf("Error: todo #%d does not exist. Nothing deleted.",delete_line);
    else 
        printf("Deleted todo #%d",delete_line);


    rewind(pfile);

    fileptr2 = fopen("replica.txt", "a");
    fseek(fileptr2,0,SEEK_SET);
    ch = getc(pfile);
    while (ch != EOF)
    {
        ch = getc(pfile);
        if (ch == '\n')
            temp++;
        if (temp != delete_line)
            putc(ch, fileptr2);
    }
    fclose(pfile);
    fclose(fileptr2);
    pfile=NULL;
    remove(filename);
    rename("replica.txt", filename);
}

// Mark as Done Handler
void done(char *n) {
    FILE *pfile = NULL;
    char *filename="todo.txt";
    FILE *fileptr2;
    char ch;
    int delete_line,count=0;
    delete_line=(int)(n[0]-'0');

    pfile = fopen(filename, "r");
    if (pfile == NULL || delete_line <= 0) {
        printf("Error: todo #%d does not exist. Nothing deleted.",delete_line);
        return;
    }

    fseek(pfile,0,SEEK_SET);
    for (ch = getc(pfile); ch != EOF; ch = getc(pfile)) 
    {
        if (ch== '\n')
            count = count + 1; 
    }

    if (delete_line <= 0 || delete_line > count )
        printf("Error: todo #%d does not exist.",delete_line);
    else
        printf("Marked todo #%d as done.",delete_line);


    rewind(pfile);

    // Pointer of file type initialized
    FILE *pfile_done = NULL;

    // Opening the file todo.txt
    // Replace todo.txt with the full path of your file
    char *filename2="done.txt";
    pfile_done=fopen(filename2,"a");
    fseek(pfile_done,0,SEEK_SET);
    fileptr2 = fopen("replica.txt", "a");
    fseek(fileptr2,0,SEEK_SET);
    ch = getc(pfile);
    count = 1;
    char line[256];
    while (fgets(line, sizeof(line), pfile)) {
        if (count == delete_line) {
            // Addint the item to the file
            fputs(line,pfile_done);
        } else {
                // Addint the item to the file
                fputs(line,fileptr2);
        }
        count++;
    }
    fclose(pfile);
    fclose(fileptr2);
    fclose(pfile_done);

    pfile_done=NULL;
    pfile=NULL;
    remove(filename);
    rename("replica.txt", filename);

}

// Reporting
void report()
{
    char cur_time[128];

  time_t      t;
  struct tm*  ptm;

  t = time(NULL);
  ptm = localtime(&t);


    int count=0;
    FILE *pfile = NULL;
    char *filename="todo.txt";
    pfile=fopen(filename,"r");
    char line[256];
    while (fgets(line, sizeof(line), pfile)) {
        count++;
    }

    int count1=0;
    FILE *pfile1 = NULL;
    char *filename1="done.txt";
    pfile1=fopen(filename1,"r");
    char line1[256];
    while (fgets(line1, sizeof(line1), pfile1)) {
        count1++;
    }

  strftime(cur_time, 128, "%Y-%m-%d", ptm);

  printf("%s Pending : %d Completed : %d", cur_time,count,count1);

   fclose(pfile);
    pfile=NULL;

    fclose(pfile1);
    pfile1=NULL;

}

int main(int argc, char* argv[])
{
    int choice = 0;

    if (argc >= 2) {
        if (strcmp(argv[1],"add")==0)       choice =  1;
        if (strcmp(argv[1],"ls")==0)        choice =  2;
        if (strcmp(argv[1],"del")==0)       choice =  3;
        if (strcmp(argv[1],"done")==0)      choice =  4;
        if (strcmp(argv[1],"report")==0)    choice =  5;
    } 

    // Handler Routes
    switch (choice) {
        case 0:
            // Call the help handler
            help();
        break;
        case 1:
            // Call the add handler
            if (argc == 2) {
                printf("Error: Missing todo string. Nothing added!\n");
                break;
            }
            add(argv[2]);
        break;
        case 2:
            // Call the ls handler
            ls();
        break;
        case 3:
            // Call the del handler
            if (argc == 2) {
                printf("Error: Missing NUMBER for deleting todo.\n");
                break;
            }
            del(argv[2]);
        break;
        case 4:
            // Call the done handler
            if (argc == 2) {
                printf("Error: Missing NUMBER for marking todo as done.\n");
                break;
            }
            done(argv[2]);
        break;
        case 5:
            // Call the report handler
            report();
        break;
    }
    return 0;
}