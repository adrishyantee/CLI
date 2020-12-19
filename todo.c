
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define MAXCHAR 100

// function definitions.
void ls();
// void del(char *n);
// void done();
void help();
// void report();

void add(char *thingToDo)
{
    printf("Added todo: \"%s\"",thingToDo);
    strcat(thingToDo,"\n");
    FILE *pfile = NULL;
    char *filename="todo.txt";
    pfile=fopen(filename,"a");
    fseek(pfile,0,SEEK_SET);
    fputs(thingToDo,pfile);
    fclose(pfile);
    pfile=NULL;
}
void help(){
printf("%s\n", "Usage :-");
printf("%s\n", "$ ./todo add \"todo item\"  # Add a new todo");
printf("%s\n", "$ ./todo ls               # Show remaining todos");
printf("%s\n", "$ ./todo del NUMBER       # Delete a todo");
printf("%s\n", "$ ./todo done NUMBER      # Complete a todo");
printf("%s\n", "$ ./todo help             # Show usage");
printf("%s\n", "$ ./todo report           # Statistics");
}

void ls(){
    int count=0;
    char c;
    FILE *pfile = NULL;
    char *filename="todo.txt";
    pfile=fopen(filename,"r");

    char str[MAXCHAR];


    for (c = getc(pfile); c != EOF; c = getc(pfile)) 
        if (c == '\n')
            count = count + 1; 

    fseek(pfile,0,SEEK_SET);
    
    while (fgets(str, MAXCHAR, pfile) != NULL)
    {
    printf("[%d] %s",count, str);
    count--;
    }
    fclose(pfile);
    pfile=NULL; 
}

// void del(char n)
// {



// }



int main(int argc, char* argv[])
{
    int a,i,p,index=0;
    if (strcmp(argv[1],"add")==0)
    a = 1;
    if (strcmp(argv[1],"ls")==0)
    a = 2;
    if (strcmp(argv[1],"del")==0)
    a = 3;
    if (strcmp(argv[1],"done")==0)
    a = 4;
    if (strcmp(argv[1],"help")==0 || strcmp(argv[1],"")==0 )
    a = 5;
    if (strcmp(argv[1],"report")==0)
    a = 6;

//SWITCH TO DIFFERENT FUNCTIONS:
switch (a){
case 1:
add(argv[2]);
break;
case 2:
ls();
break;
// case 3:
// del();
// case 4:
// done();
case 5:
help();
break;
// case 6:
// report();
}
    return 0;
}