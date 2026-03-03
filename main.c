#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 150
#define MAX_SIZE 100

//Hiba Ala Ali Hmidan 1230454
//section 4

/*
This project is a task management system, it will read tasks from a file and add them to a list,
when the user performs a task, the task will be moved to a stack,
when the user undo a task, it will move the recently performed task back to the end of the list, with an indicator that it is undone, in addition to adding it to undone list which saves all tasks that have been undone
*/

typedef struct node* PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Stack;

struct node{
    int ID;
    char* name;
    char* date;
    double duration;
    int isUndone;
    struct node* next;
};


typedef struct QueueRecord* Queue;
struct QueueRecord{
    int front;
    int rear;
    int size;
    int capacity;
    PtrToNode *arr;
};
//prototypes for functions used
void deleteList(struct node* L);
struct node* makeEmpty(struct node* L);
int isEmpty(List L);
int isLast(PtrToNode p,List L);
PtrToNode findByID(int id,List L);
PtrToNode findPrevious(int id,List L);
void deleteTask(List L);
PtrToNode createNode(int ID,char* name,char* date,double duration);
void insertAtEnd(PtrToNode newNode,List L);
void displayMenu();
void loadTasksFileToList(List L,Stack S);
PtrToNode findByName(char* Name,List L);
void SearchforATask(List L);
int IdExistsInList(int id,List L);
int IdExistsInStack(int id,Stack S);
void addTask(List L,Stack S);
void printTaskInfo(PtrToNode p);
void printUnperformed(List L);
Stack CreateStack();
int isEmptyStack(Stack S);
PtrToNode Pop(Stack S);
void Push(PtrToNode p,Stack S);
void MakeEmpty(Stack S);
void DisposeStack(Stack S);
void PrintPerformedStack(Stack S);
void removeNode(PtrToNode ptr,List L);
void performTask(List L,Stack S);
void undoLastPerfTask(List L1,Stack S,Queue Q);
void generateSummaryReport(List L1,Stack S,Queue Q);
void printTaskOnFile(PtrToNode task,FILE *ptr);
void printUnperformedOnFile(List L,FILE *ptr);
void printUndoneOnFile(Queue Q,FILE *ptr);
void printCurrentUndoneOnFile(List L,FILE *ptr);// i did not use this function but it prints only current undone
                                                // I will explain difference between both in discussion
void PrintPerformedOnFile(Stack S,FILE *ptr);
int isEmptyQueue(Queue Q);
int isFullQueue(Queue Q);
void MakeEmptyQueue(Queue Q);
Queue CreateQueue(int MaxElements);
int Succ(int value,Queue Q);
void Enqueue(PtrToNode x,Queue Q);
PtrToNode Front(Queue Q);
void Dequeue(Queue Q);


int main()
{
     List mainList=NULL;// This is the main Linked List in our program
     mainList=makeEmpty(mainList);
     Stack performedStack=CreateStack();// This stack will have the performed tasks
     Queue undoneQueue=CreateQueue(MAX_SIZE);//This queue will track all undone movements
     int choice;
     printf("Welcome to our Task Management System!\n");
    do{
        displayMenu();
        scanf("%d",&choice);
        printf("\n");
        switch(choice){
        case 1:loadTasksFileToList(mainList,performedStack);
               break;
        case 2:addTask(mainList,performedStack);
               break;
        case 3:deleteTask(mainList);
               break;
        case 4:SearchforATask(mainList);
               break;
        case 5:printUnperformed(mainList);
               break;
        case 6:performTask(mainList,performedStack);
               break;
        case 7:undoLastPerfTask(mainList,performedStack,undoneQueue);
               break;
        case 8:PrintPerformedStack(performedStack);
               break;
        case 9:generateSummaryReport(mainList,performedStack,undoneQueue);
               break;
        case 10:printf("Exiting program...Bye!\n");
                break;
        default:printf("please enter a valid choice from 1 to 10!\n");
        }

    }while(choice!=10);


    return 0;
}

void deleteList(struct node* L){// This function deletes a Linked List
    struct node *p,*temp;
    p=L->next;
    L->next=NULL;
    while(p!=NULL){
        temp=p->next;
        free(p);
        p=temp;
    }
}

struct node* makeEmpty(struct node* L){// This function creates an empty list
    if(L!=NULL)
        deleteList(L);
    L=(struct node*)malloc(sizeof(struct node));
    if(L==NULL){
        printf("out of memory!\n");
        return NULL;
    }
    L->next=NULL;
    return L;
}

int isEmpty(List L){//To check if a List is empty
      return L->next==NULL;
}

int isLast(PtrToNode p,List L){//To check if a node is the last node in the List
    return p->next==NULL;
}

PtrToNode findByID(int id,List L){// This returns a pointer to node which has the Id that we are looking for
    if(L==NULL )
    {
        printf("List is NULL\n");
        return NULL;
    }
    struct node* p=L->next;
    while(p!=NULL && p->ID!=id)//this stops when we reach the end of the list or when we find the ID
        p=p->next;
    return p;
}

PtrToNode findPrevious(int id,List L){// This returns a pointer to node which is before the node that has the Id that we are looking for
    if(L==NULL){
        printf("List is NULL");
        return NULL;
    }
    struct node *p=L;
    while(p->next!=NULL && p->next->ID!=id)//this stops when we reach the last node of the list or when we find the previous node to the node that has the ID
        p=p->next;
    return p;
}

void deleteTask(List L){// This function deletes a Task and free it from memory
   if(L==NULL)
       printf("list is NULL!\n");
   else if(isEmpty(L))
       printf("No tasks to Delete!\n");
   else{
    int id;
    printf("please enter the ID to delete: ");
    scanf("%d",&id);

    PtrToNode prev=findPrevious(id,L);
    if(!isLast(prev,L)){
    PtrToNode temp=prev->next;
    prev->next=temp->next;
    free(temp);
    printf("deleted successfully!\n");
   }
   else printf("couldn't find the task!\n");
   }

}

PtrToNode createNode(int ID,char* name,char* date,double duration){//this method creates a newNode with all its information
    PtrToNode newNode=(PtrToNode)malloc(sizeof(struct node));// we allocate memory for a new node
    if(newNode==NULL){// this means that our memory is out of space
        printf("out of memory!\n");
        return NULL;
    }
    newNode->ID=ID;
    newNode->name=strdup(name);// this allocates and copies string name into node's name
    newNode->date=strdup(date);
    newNode->duration=duration;
    newNode->isUndone=0;//any new created task is marked as undone
    newNode->next=NULL;

    return newNode;

}

void insertAtEnd(PtrToNode newNode,List L){// This function inserts a task at the end of the List
    if(newNode==NULL || L==NULL)
        printf("Either list is NULL or newNode is NULL");
    else{
      PtrToNode p=L;
      while(p->next!=NULL)
            p=p->next;
       p->next=newNode;
    }
}

void displayMenu(){
    printf("\nMenu:\n");
    printf("1. Load Tasks File\n");
    printf("2. Add a New Task\n");
    printf("3. Delete a Task \n");
    printf("4. Search for a Task\n");
    printf("5. View Unperformed Tasks\n");
    printf("6. Perform a Task\n");
    printf("7. Undo Last Performed Task\n");
    printf("8. View Performed Tasks\n");
    printf("9. Generate Summary Report\n");
    printf("10. Exit\n");
    printf("Please enter your choice: ");
}

void loadTasksFileToList(List L,Stack S){
    if(L==NULL){
        printf("List is NULL");
        return;
    }
    FILE *file=fopen("tasks.txt","r");
    if(file==NULL){// if we could not find the file or the file name was incorrect
        printf("File not found\n");
        return;
    }
    else{
       char line[MAX_LINE_LENGTH];// I used it to scan a line of the file
       int count,id;
       char *name,*date;
       double time;
       while(fgets(line,MAX_LINE_LENGTH,file)){
        count=0;

        char *portion=strtok(line,"#");// to divide the line based on #
        while(portion!=NULL){
            count++;
            switch(count){
            case 1:id=atoi(portion);//ASCII to integer
                   break;
            case 2:name=portion;
                   break;
            case 3:date=portion;
                   break;
            case 4:time=atof(portion);//ASCII to float(double)
                   break;
            }
            portion=strtok(NULL,"#");
        }
        if(count!=4){
            printf("file format for a line is incorrect!\n");// for example if the a line was: 15#submit project 1#5/4/2025#0.1#data , this is incorrect
        }
        else{
            if(!IdExistsInList(id,L)&& !IdExistsInStack(id,S)){// to check if the task was already added, because if the user pressed on load file twice, we don't want to add the tasks again
              PtrToNode t=createNode(id,name,date,time);
              insertAtEnd(t,L);
            }
            else printf("id %d already exists!\n",id);
        }
       }
         fclose(file);
         printf("Loaded data from file successfully!\n");
    }

}

PtrToNode findByName(char* Name,List L){//This is used to find a task based on its name that is entered by a user
    if(L==NULL )
    {
        printf("List is NULL\n");
        return NULL;
    }
    PtrToNode p=L->next;
    while(p!=NULL && strcmp(Name,p->name)!=0)
        p=p->next;
    return p;
}

void SearchforATask(List L){// This function will give the user the option to search for a task either by Id or by name
    int choice,id;
    char name[100];
    PtrToNode p;
    printf("Enter 1 to search by task ID or Enter 2 to search by task Name: ");
    scanf("%d",&choice);
    if(choice==1){// if the user chose to search by ID
        printf("Enter task ID: ");
        scanf("%d",&id);
        p=findByID(id,L);
        if(p==NULL) printf("Task not found\n");
        else{
            printf("\nTask information:\n");
            printTaskInfo(p);// This calls a function that prints the task information in a good format
        }
    }
    else if(choice==2){// if the user chose to search by name
        printf("Enter task Name: ");
        getchar();//to clear new line
        gets(name);
        p=findByName(name,L);
        if(p==NULL) printf("Task not found\n");
        else{
            printf("\nTask information:\n");
            printTaskInfo(p);
        }
    }
    else printf("invalid entered number!\n");// if the user did not enter 1 nor 2
}




int IdExistsInList(int id,List L){// This function returns 1 if the Id is already used in the Linked List
    if(L->next==NULL) return 0;
    else if(L->next->ID==id) return 1;
    else return IdExistsInList(id,L->next);//By recursion
}

int IdExistsInStack(int id,Stack S){// This function returns 1 if the Id is already used in the performed Stack
    int flag=0;
    if(isEmptyStack(S)) // if Stack is Empty then it returns 0 because the Id will not be found
       return 0;
    Stack newS=CreateStack();// in stacks we are only allowed to use push and pop, so i created a new stack to move elements to it while searching for id
    while(!isEmpty(S)){
        PtrToNode temp=Pop(S);
        if(temp->ID==id)
            flag=1;// this indicate that the id was found
        Push(temp,newS);
    }
    while(!isEmpty(newS))// return the tasks back to their original stack
        Push(Pop(newS),S);
    return flag;// return whether the id was found or not
}

void addTask(List L,Stack S){// adds a new task to the system, this function took list and stack as parameters to check if the new id already exists in our system
   int ID;
   char name[100],date[20];
   double time;
   printf("Please enter Task information below: \n");
   printf("ID: ");
   scanf("%d",&ID);

   while(IdExistsInList(ID,L)||IdExistsInStack(ID,S)){// check if ID has already been given to a previous task
    printf("ID already exists, please enter another ID:");
    scanf("%d",&ID);
   }

   printf("Name : ");
   getchar();//clear newline
   gets(name);

   printf("Date(DD/MM/YYYY) : ");
   gets(date);

   printf("Duration : ");
   scanf("%lf",&time);
   PtrToNode t=createNode(ID,name,date,time);// this creates a new node to add to the linked list
   insertAtEnd(t,L);// this will insert the task in a node add the end of the list
}

void printTaskInfo(PtrToNode p){// prints task info with its details
    printf("ID:   %d\n",p->ID);
    printf("Name: %s\n",p->name);
    printf("Date: %s\n",p->date);
    printf("Duration:%.2f\n",p->duration);
}

void printUnperformed(List L){
     if(L==NULL)
        printf("List is NULL");
    else if(isEmpty(L))
        printf("no unperformed tasks!\n");
     else{
         PtrToNode P=L->next;
         int count=0;
         while(P!=NULL){
            if(P->isUndone==0)
              printf("\nTask #%d info:\n",++count);// print unperformed tasks
            else
              printf("\nTask #%d info:  (This task was undone and moved to unperformed)\n",++count);// indicate that this task is in unperformed  list but it was undone,
                                                                    // because the meaning of unperformed and undone are similar but slightly different, where unperformed task may mean that it was never started, but undone means not yet completed
         printTaskInfo(P);
         P=P->next;
     }
}
}

Stack CreateStack(){// create an new stack function
    Stack S=(struct node*)malloc(sizeof(struct node));
    if(S==NULL){
        printf("out of memory\n");
        return NULL;
    }
    S->next=NULL;

    return S;
}

int isEmptyStack(Stack S){ // returns 1 if stack is empty
    return S->next==NULL;
}

PtrToNode Pop(Stack S){// this Pop function will remove the Top element in the Stack and returns it as a Task struct
    if(!isEmptyStack(S))
    {
        PtrToNode firstCell=S->next;
        S->next=firstCell->next;
        firstCell->next=NULL;
        return firstCell;
    }
    else{
        printf("stack empty, cannot pop\n");
        return NULL;//if the Stack was empty
    }
}

void MakeEmpty(Stack S){// this function makes the stack empty
    if(S==NULL)
        printf("out of space\n");
    else
        while(!isEmptyStack(S))
           Pop(S);
}

void Push(PtrToNode p,Stack S){// This pushes a task into the stack
    if(S==NULL)
        printf("Stack is NULL!\n");
    else
    {
        p->next=S->next;
        S->next=p;
    }
}

void DisposeStack(Stack S){// to dispose a stack
    MakeEmpty(S);
    free(S);
}

void PrintPerformedStack(Stack S){//print performed tasks from first performed to last performed
    if(S==NULL)
         printf("Stack is NULL\n");
    else  if(isEmptyStack(S))
        printf("No performed tasks at the moment!\n");
    else{
    int count=1;
    Stack newS=CreateStack();// we are only allowed to use Push and Pop when dealing with stacks so I created a new stack
    printf("Performed Tasks from first performed to last performed:\n");
    while(!isEmptyStack(S))
        Push(Pop(S),newS);

    while(!isEmptyStack(newS)){
        PtrToNode temp=Pop(newS);
        printf("\nTask #%d info:\n",count++);// this will print them in order from first performed to last
        printTaskInfo(temp);
        Push(temp,S);
    }
    }
}

void removeNode(PtrToNode ptr,List L){// removes a node from the list without free memory because we will use it in the performTask function
   if(isEmpty(L)) printf("List is empty!\n");
   else{
     PtrToNode p=L;
     while(p->next!=ptr)
        p=p->next;
     p->next=ptr->next;
    }
}

void performTask(List L,Stack S){// this performs a task based on its ID by removing it form list and moving it to a Stack
    int id;
    printf("please enter task ID to perform: ");
    scanf("%d",&id);
    PtrToNode p=findByID(id,L);
    if(p==NULL) printf("Task not found!\n");
    else{

        removeNode(p,L);// this removes the node from the List
        Push(p,S);// this Pushes the task into the performed stack
        printf("Task performed successfully!\n");
    }
}

void undoLastPerfTask(List L1,Stack S,Queue Q){
    if(isEmptyStack(S))// if there was no performed tasks to undo
        printf("no performed tasks!\n");
    else
    {
        PtrToNode last=Pop(S);//get the most recently performed task
        last->isUndone=1;// mark the task as undone
        insertAtEnd(last,L1);// insert the task at the end of the list

        PtrToNode un=createNode(last->ID,last->name,last->date,last->duration);// this adds the undone task into an undone list which stores all our undone tasks
        Enqueue(un,Q);

        printf("Undone Last performed task successfully!\n");
    }
}

void generateSummaryReport(List L1,Stack S,Queue Q){//This function generates an output file
    FILE *file=fopen("Report.txt","w");
    if(file==NULL){
        printf("couldn't open file\n");
        return;// return if the file could not open
    }
    fprintf(file,"Summary Report:\n\n");
    printUnperformedOnFile(L1,file);
    printUndoneOnFile(Q,file);
   // printCurrentUndoneOnFile(L1,file);// This function prints only current undone tasks, not all undone movements
    PrintPerformedOnFile(S,file);
    printf("Report Generated!\n");
    fclose(file);
}

void printTaskOnFile(PtrToNode task,FILE *ptr){// this function prints a task on file in a good format
    fprintf(ptr,"ID:   %d\n",task->ID);
    fprintf(ptr,"Name: %s\n",task->name);
    fprintf(ptr,"Date: %s\n",task->date);
    fprintf(ptr,"Duration:%.2f\n",task->duration);
}

void printUnperformedOnFile(List L,FILE *ptr){// this function prints the unperformed tasks with their details on the output file
    PtrToNode P=L;
    int count=0;
    if(L==NULL) // if the list was NULL
        fprintf(ptr,"could not find List!\n");
    else if(isEmpty(L))
        fprintf(ptr,"no unperformed tasks!\n");
    else{
        fprintf(ptr,"Unperformed tasks:\n");

        while(P->next!=NULL){
            P=P->next;
            if(P->isUndone==0)// print the tasks marked as unperformed( not including undone tasks)
                fprintf(ptr,"\nTask #%d info:\n",++count);
            else
                fprintf(ptr,"\nTask #%d info:  (This task is currently undone and also stored in unperformed list)\n",++count);
           printTaskOnFile(P,ptr);
        }
        fprintf(ptr,"\n");
    }
}

void printUndoneOnFile(Queue Q,FILE *ptr){// this function prints all tasks that have been undone with their details on the output file

    if(Q==NULL)
        fprintf(ptr,"Queue not found");
    else if(isEmptyQueue(Q))
        fprintf(ptr,"\nNo Undone tasks\n");
    else{
        int count=0;
        fprintf(ptr,"\nAll tasks that have been undone in our system (Track of all undone movements from first to last even if they got re-performed):  \n");
                            // The function below this only prints current undone, i will explain in discussion which one to choose
        Queue temp=CreateQueue(MAX_SIZE);
         while(!isEmptyQueue(Q))
        {
           fprintf(ptr,"\nTask #%d info:\n",++count);
           printTaskOnFile(Front(Q),ptr);
           Enqueue(Front(Q),temp);
           Dequeue(Q);
        }
        while(!isEmptyQueue(temp))
        {
            Enqueue(Front(temp),Q);
            Dequeue(temp);
        }

    }
}

// below is a function that prints only the current undone tasks

void printCurrentUndoneOnFile(List L,FILE *ptr){// this function prints the undone tasks with their details on the output file

    if(L==NULL)
        fprintf(ptr,"List is NULL");
    else{
        PtrToNode p=L->next;
        int count=0;
        fprintf(ptr,"Undone tasks:\n");
        while(p!=NULL){
            if(p->isUndone==1)// print the tasks that are marked as undone
            {
                fprintf(ptr,"\nTask #%d info:\n",++count);
                printTaskOnFile(p,ptr);
            }
            p=p->next;
        }
        if(!count)
             fprintf(ptr,"No Undone tasks\n");
    }
}


void PrintPerformedOnFile(Stack S,FILE *ptr){// This function prints the performed task
     if(S==NULL)
        fprintf(ptr,"Stack is NULL\n");
     else if(isEmptyStack(S))
        fprintf(ptr,"\nNo Performed Task\n");// if the stack is empty
     else{
        fprintf(ptr,"\nPerformed Tasks: (from most recently perform)\n");
        Stack newS=CreateStack();// I created a new stack to temporarily move elements to it because we are only allowed to use push and pop when dealing with stacks
        int count=0;// this is just used to print tasks for example: "Task #1 info:"
     while(!isEmptyStack(S)){
        PtrToNode temp=Pop(S);
        fprintf(ptr,"\nTask #%d info:\n",++count); // print task info of the top task
        printTaskOnFile(temp,ptr);
        Push(temp,newS);// push it to newStack
    }
    fprintf(ptr,"\n");
    while(!isEmptyStack(newS))// push the tasks back to their original stack
        Push(Pop(newS),S);
     }
}


int isEmptyQueue(Queue Q){
    return Q->size==0;
}

int isFullQueue(Queue Q){
    return Q->size==Q->capacity;
}

void MakeEmptyQueue(Queue Q){
    Q->front=1;
    Q->rear=0;
    Q->size=0;
}

Queue CreateQueue(int MaxElements){// function to create a new Queue
    Queue q;
    q=(Queue)malloc(sizeof(struct QueueRecord));
    if(q==NULL) printf("out of space!\n");
    q->arr=(PtrToNode *)malloc(sizeof(struct node)*MaxElements);
    if(q->arr==NULL) printf("out of space\n");
    q->capacity=MaxElements;
    MakeEmptyQueue(q);
    return q;
}

int Succ(int value,Queue Q){
    if(++value==Q->capacity)
        value=0;
    return value;
}

void Enqueue(PtrToNode x,Queue Q){
    if(!isFullQueue(Q)){
        Q->size++;
        Q->rear=Succ(Q->rear,Q);
        Q->arr[Q->rear]=x;
    }
    else printf("Full Queue");
}

PtrToNode Front(Queue Q){
   if(!isEmptyQueue(Q))
     return Q->arr[Q->front];
   printf("Empty Queue");
   return NULL;
}

void Dequeue(Queue Q){
    if(!isEmptyQueue(Q))
    {
        Q->size--;
        Q->front=Succ(Q->front,Q);
    }
    else printf("Empty Queue!");
}
