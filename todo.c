#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>

#define MAXCHAR 100

// You can replace the three text file paths below with any custom path of your choice, the app will still work as needed!
char *todo_file="./todo.txt";
char *done_file="./done.txt";
char *temp_file="./replica.txt";

/*
    HELP handler
    This function displays the help commands.
*/
void help() {

    // Note that the same code as below has been found to not pass on windows based systems but are working fine in non windows based systems.
    printf("%s\n", "Usage :-");
    printf("%s\n", "$ ./todo add \"todo item\"  # Add a new todo");
    printf("%s\n", "$ ./todo ls               # Show remaining todos");
    printf("%s\n", "$ ./todo del NUMBER       # Delete a todo");
    printf("%s\n", "$ ./todo done NUMBER      # Complete a todo");
    printf("%s\n", "$ ./todo help             # Show usage");
    printf("%s\n", "$ ./todo report           # Statistics");
}

/*
    ADD handler
    This function takes input - the todo item, as string and adds it to the file `todo.txt` / creates it if it doesn't exist.
*/
void add(char *item) {
    // Log the success message
    printf("Added todo: \"%s\"",item);

    // Concatenate a new line
    strcat(item,"\n");

    // Pointer of file type initialized
    FILE *pfile = NULL;

    // Opening the file todo.txt
    // Replace todo.txt with the full path of your file
    
    pfile=fopen(todo_file,"a");

    // Setting the position to the beginning of the file to write
    fseek(pfile,0,SEEK_SET);

    // Addint the item to the file
    fputs(item,pfile);

    // Closing the file
    fclose(pfile);

    pfile=NULL;
}

/* List ToDo Handler
*/

void ls(){

    // Variables init
    int count=0,i=0,j=0;
    char c;
    FILE *pfile = NULL;
    
    // Open the todo file in read mode
    pfile=fopen(todo_file,"r");

    // If file doesn't exist, display an appropriate way
    if (pfile == NULL) {
        printf("There are no pending todos!\n");
        return;
    }

    // Init array to store the individual todos
    char str[20][MAXCHAR];

    // Loop through the file content and extract the ToDos
    for (c = getc(pfile); c != EOF; c = getc(pfile)) 
     {
        // \n marks the end of a todo
        if (c == '\n') {
            // End the current storage counter and jump to the next counter for the next ToDo
            str[i][j] = '\0';
            count = count + 1; 
            i++;
            j=0;
        } else {
            // Else continue to store the characters in the current counter
            str[i][j++]=c;
        }
     }

    // Print the extracted ToDos in reverse order.
    for (i=count-1;i>=0;i--) {
        printf("[%d] %s\n",i+1, str[i]);
    }  

    // Close the file
    fclose(pfile);
    pfile=NULL; 
}

// DELETE ToDo Handler
void del(char *n)
{
    // Init variables
    char ch;
    int delete_line, temp = 1,count=0;
    FILE *pfile = NULL;
    FILE *fileptr2;

    // Extract the ToDo number from the arguments
    delete_line=(int)(n[0]-'0');

    // Open the todo file and display appropriate message if no ToDos exist
    pfile = fopen(todo_file, "r");
    if (pfile == NULL || delete_line <= 0) {
        printf("Error: todo #%d does not exist. Nothing deleted.",delete_line);
        return;
    }

    // Move the file position of the stream to the starting of the opened file
    fseek(pfile,0,SEEK_SET);

    // Loop through the contents and count the number of ToDos
    for (ch = getc(pfile); ch != EOF; ch = getc(pfile)) 
    {
        if (ch== '\n')
            count = count + 1; 
    }

    // Check if argument passed is valid
    if (delete_line <= 0 || delete_line > count )
        printf("Error: todo #%d does not exist. Nothing deleted.",delete_line);
    else 
        printf("Deleted todo #%d",delete_line);

    // Set the file position of the stream to the beginning of the file again.
    rewind(pfile);

    /*
        Here, we create a new file named replica.txt and transfer all those ToDos which the user doesn't want to delete.

        After the process ends, we simply delete the todo.txt file and rename replica.txt as todo.txt
    */

    // Open the temp file (or exist if it isn't present)
    fileptr2 = fopen(temp_file, "a");

    // Move the file position of the stream to the starting of the file
    fseek(fileptr2,0,SEEK_SET);

    // Extract the characters and move them sequentially to the appropriate file, depending on the todo number
    ch = getc(pfile);
    while (ch != EOF)
    {
        // temp variable stores the number of the current todo
        if (ch == '\n')
            temp++;

        // If temp != the deleting todo number, add the characters to the replica file
        if (temp != delete_line)
            putc(ch, fileptr2);

        // Incrementing the loop
        ch = getc(pfile);
    }

    // Close the todo and replica files
    fclose(pfile);
    fclose(fileptr2);
    pfile=NULL;

    // Delete todo.txt
    remove(todo_file);

    // Rename replica.txt as todo.txt
    rename(temp_file, todo_file);
}

// Mark as Done Handler
void done(char *n) {

    // Init variables
    char ch;
    int delete_line,count=0;
    FILE *pfile = NULL;    
    FILE *fileptr2;

    // Extract the todo number from the arguments
    delete_line=(int)(n[0]-'0');

    // Open the file and display appropriate message if no ToDo exist
    pfile = fopen(todo_file, "r");
    if (pfile == NULL || delete_line <= 0) {
        printf("Error: todo #%d does not exist. Nothing deleted.",delete_line);
        return;
    }

    // Set the file position of the stream to the beginning.
    fseek(pfile,0,SEEK_SET);

    // Loop through the contents and count the number of lines
    for (ch = getc(pfile); ch != EOF; ch = getc(pfile)) 
    {
        if (ch== '\n')
            count = count + 1; 
    }

    // Check if the line to be marked as done is valid
    if (delete_line <= 0 || delete_line > count )
        printf("Error: todo #%d does not exist.",delete_line);
    else
        printf("Marked todo #%d as done.",delete_line);


    // Set the file position of the stream to the beginning again.
    rewind(pfile);

    // Pointer of file type initialized
    FILE *pfile_done = NULL;

    // Open the done.txt file and set position to start
    pfile_done=fopen(done_file,"a");
    fseek(pfile_done,0,SEEK_SET);

    // Open the replica.txt file and set position to start 
    fileptr2 = fopen(temp_file, "a");
    fseek(fileptr2,0,SEEK_SET);

    count = 1;
    char line[256];

    // Loop through the lines and move to done.txt or replica.txt accordingly
    while (fgets(line, sizeof(line), pfile)) {
        if (count == delete_line) {
            // Adding the item to the done file
            fputs(line,pfile_done);
        } else {
                // Adding the item to the replica file
                fputs(line,fileptr2);
        }
        count++;
    }

    // Close all three files
    fclose(pfile);
    fclose(fileptr2);
    fclose(pfile_done);

    pfile_done=NULL;
    pfile=NULL;

    // Delete todo.txt and rename replica.txt as todo.txt
    remove(todo_file);
    rename(temp_file, todo_file);

}

// REPORT Handler
void report()
{

    // Initialize variables
    char cur_time[128];
    time_t t;
    int count=0, count1 = 0;
    struct tm*  ptm;
    t = time(NULL);
    FILE *pfile = NULL;

    // Store the local time
    ptm = localtime(&t);
    
    // Open the todo file and count the number of lines
    pfile=fopen(todo_file,"r");
    char line[256];
    while (fgets(line, sizeof(line), pfile)) {
        count++;
    }

    FILE *pfile1 = NULL;

    // Open the done file and count the number of lines
    pfile1=fopen(done_file,"r"); 
    char line1[256];
    while (fgets(line1, sizeof(line1), pfile1)) {
        count1++;
    }

    // Format the date as required
    strftime(cur_time, 128, "%Y-%m-%d", ptm);

    // Printing the output
    printf("%s Pending : %d Completed : %d", cur_time,count,count1);

    // Close the two files
    fclose(pfile);
    pfile=NULL;

    fclose(pfile1);
    pfile1=NULL;
}

int main(int argc, char* argv[])
{
    // Initialize choice variable, default -> 0 for displaying help
    int choice = 0;

    // If more than 1 argument is passed, store the appropriate choice 
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
            // Display a message if the addition todo string isnt provided
            if (argc == 2) {
                printf("Error: Missing todo string. Nothing added!\n");
                break;
            }
            // Call the add handler
            add(argv[2]);
        break;
        case 2:
            // Call the ls handler
            ls();
        break;
        case 3:
            // Display a message if the deletion todo number isnt provided
            if (argc == 2) {
                printf("Error: Missing NUMBER for deleting todo.\n");
                break;
            }
            // Call the del handler
            del(argv[2]);
        break;
        case 4:
            // Display a message if the mark as done todo number isnt provided
            if (argc == 2) {
                printf("Error: Missing NUMBER for marking todo as done.\n");
                break;
            }
            // Call the done handler
            done(argv[2]);
        break;
        case 5:
            // Call the report handler
            report();
        break;
    }
    return 0;
}