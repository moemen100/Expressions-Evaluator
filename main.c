#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define STACKSIZE 20

typedef struct //Structure for stack.
{
    int top;
    float items[STACKSIZE];
} stack;

typedef struct //Structure for expressions' data.
{
    char var[20]; //Expression name.
    char mexp[60]; //Infix expression.
    char pexp[60]; //Postfix expression.
    char result[10]; //Expression result.
} exp;

typedef struct //Structure for the infix expressions' partition.
{
    char part[10]; //Operands.
    char oper[1]; //Operators.
} part;

FILE *p ; //A file to store the data.

exp arexp[20]; //Array of Structure.
part arpart[20]; //Array of Structure.

int cm; //The infix expression length.
int n=-1; //The number of expressions available.
int r=0; //Prevent infinite looping.
float total; //To evaluate multiple digits.

int isFull(stack *s); //Stack implementation.
int Priority(char x);  //Check the operator's priority
int isEmpty(stack *s); //Stack implementation.

void Add(); //Add a new variable.
void cpy();  //Copy the infix expression into a temporary array.
void error(); //Manage all user's input errors.
void Modify(); //Modify expression's name.
void Redirect(); //Redirects to main menu.
void Main_Menu(); //Menu.
void Exit_Menu(); //Menu.
void Print_All(); //Print all available expressions.
void Print_Sum(); //Print the sum of all available expressions.
void Save_File(); //Save entered expressions in an external file.
void Load_File(); //Load expressions saved in an external file.
void Reset_File(); //Remove all data stored in the file.
void Delete_All(); //Remove all existing expressions.
void Print_Menu(); //Menu.
void Start_Menu(); //Menu.
void Delete_Menu(); //Menu.
void Delete_File(); //Delete stored file.
void Create_File(); //Create new file.
void Print_Number(); //Show the number of expressions.
void Print_Sorted(); //Print all available expressions after sorting.
void Print_Average(); //Show the average of all available expressions.
void Print_Max_Min(); //Print the maximum and minimum expressions.
void Manage_File_Menu(); //Menu.
void save(char infix[]); //Save the entered expression in an array of Structure, as expression name and infix expression.
void parts(char infix[]); //Partition of the infix expression into operands and operators.
void intialize(stack *s); //Stack implementation.
void Delete_Expression(); //Delete an expression by entering it's name.
void search(char infix[]); //Check if a variable exists in the infix expression to substitute with its value.
void push(stack *s,float value); //Stack implementation.
void Check_Repeated_Name(char infix[]); //Manage an error.

float pop(stack *s); //Stack implementation.
float Top(stack *s); //Stack implementation.
float Infix_to_Postfix_Evaluation(char infix[]); //Infix>Postfix>Evaluate.

void main()
{
    system("color 3e"); //Background and font colors.
    Start_Menu();
}

void intialize(stack *s)
{
    s->top=0; //Change the index of the top item to 0.
}

int isFull(stack *s)
{
    return s->top<STACKSIZE?0:1; //Return 1 when the stack is full.
}

int isEmpty(stack *s)
{
    return s->top==0?1:0; //Return 1 when the stack is empty.
}

float pop(stack *s)
{
    return s->items[--s->top]; //Remove the last item from the stack.
}

void push(stack *s,float value)
{
    s->items[s->top++]=value; //Push an item into the top of the stack.
}

float Top(stack *s)
{
    float x=pop(s);
    push(s,x);
    return x; //Return the value of the top item in the stack.
}

void error()
{
    n--; //Remove the recently added incorrect expression.
    printf("\n");
    Redirect();
}

void Redirect()
{
    printf("Press any key to return to main menu.\n");
    getch();
    Main_Menu(); //Returns to main menu.
}

void Add()
{
    char infix[60];
    printf("Enter the expression : ");
    gets(infix); //Input expression.
    search(infix);
    Check_Repeated_Name(infix);
    printf("\n");
    printf("Expression has been added successfully.\n");
    Redirect();
}
int Priority(char x)
{
    switch(x)
    {
    case '(':
        return 3;
        break;
    case '*':
        return 2;
        break;
    case '/':
        return 2;
        break;
    case '+':
        return 1;
        break;
    case '-':
        return 1;
        break;
    default:
        return 4;
    }
}

void search(char infix2[])
{
    int i,j;
    save(infix2);
    if(n!=0) //If there're more than one expression.
    {
        for(i=0; i<=20; i++)
        {
            for (j=0; j<=10; j++)
                arpart[i].part[j]='\0';
            arpart[i].oper[0]='\0';
        }
        parts(arexp[n].mexp);
        for(i=0; i<n; i++)
            for(j=0; j<=cm; j++)
            {
                if (strcmp(arexp[i].var,arpart[j].part)==0) //Checks if a variable exists in the infix expression.
                    strcpy(arpart[j].part,arexp[i].result); //Substitute the variable by its value.
            }
        cpy();
    }
}

void save(char infix[])
{
    n++; //Increase the number of expressions.
    int f=0;
    int c=0,i;
    if(strlen(infix)==0)
    {
        n--;
        printf("\n");
        printf("No entry found.\n");
        printf("Press any key to return to main menu.\n");
        getch();
        Main_Menu();
    }
    for (c=0; c<strlen(infix); c++)
        if(infix[c]=='=')
            f=1; //A flag to check if the equal sign exists in the expression.

    if (f==0)
    {
        printf("\n");
        printf("Error..\nAn equal sign (i.e. = ) must be included \nbetween the variable name and the infix expression.\n");
        error();
    }

    f=0;
    c=0;

    while(infix[c]!='=')
    {
        if(infix[c]==' ')
            f=1; //A flag to check if there're spaces in the expression's name.
        c++;
    }
    if (f==1)
    {
        printf("\n");
        printf("Error..\nExpression name can't contain spaces.\n");
        error();
    }

    f=0;
    c=0;
    while(infix[c]!='=')
    {
        if(isalpha(infix[c]))
            f=1; //A flag to check if there're alphabetical characters in the expression's name.

        if(infix[c]=='+'||infix[c]=='-'||infix[c]=='*'||infix[c]=='/'||infix[c]=='('||infix[c]==')')
        {
            printf("\n");
            printf("Error..\nThe variable name can't contain spaces , parentheses \nor the operators characters.\n");
            printf("(i.e. (, ), +, -, * & /).\n");
            error();
        }
        else
        {
            arexp[n].var[c]=infix[c]; //Store the variable name in the array of structure.
            c++;
        }
    }

    if (f==0)
    {
        printf("\n");
        printf("Error..\nThe variable name must contain an alphabet.\n");
        error();
    }

    for(i=c+1; i<=strlen(infix); i++)
        arexp[n].mexp[i-(c+1)]=infix[i]; //Store the infix expression in the array of structure.

    infix[i]='\0';
    if (n==0)
        sprintf(arexp[n].result,"%.2f",Infix_to_Postfix_Evaluation(arexp[n].mexp));
}

void parts(char infix[])
{
    int i,j=0,c=0;
    for(i=0; i<strlen(infix); i++)
    {
        while(infix[i]!='+'&&infix[i]!='*'&&infix[i]!='-'&&infix[i]!='/'&&infix[i]!='\0'&&infix[i]!=')'&&infix[i]!='(')
        {
            arpart[c].part[j]=infix[i]; //Store the digits/variables in an array of structure.
            i++;
            j++;
        }
        arpart[c].part[j]='\0';
        if(infix[i]=='+'||infix[i]=='*'||infix[i]=='-'||infix[i]=='/'||infix[i]=='('||infix[i]==')')
        {
            arpart[c].oper[0]=infix[i]; //Stores the operators in an array of structure.
            c++;
            j=0;
        }
    }
    cm=c; //The number of operators in the expression.
}

void cpy()
{
    char temp[60];
    int i,j=0,i1=0;
    if(cm>=0)
    {
        for(i=0; i<=cm; i++)
        {
            while(arpart[i].part[j]!='\0')
            {
                temp[i1]=arpart[i].part[j];
                j++;
                i1++;
            }
            if(i<cm)
            {
                temp[i1]=arpart[i].oper[0];
                i1++;
            }
            j=0;
        }
        temp[i1]='\0';
        sprintf(arexp[n].result,"%.2f",Infix_to_Postfix_Evaluation(temp)); //Store the evaluated result in the array of structure after changing its type into float.
    }
}

float Infix_to_Postfix_Evaluation(char temp0[])
{
    stack s;
    intialize(&s);
    char postfix[60];
    char infix[60];
    int i=0;
    int j=0;
    int q=0;
    float x;
    float a,b;
    for(i=0; i<strlen(temp0); i++)
        if(temp0[i]=='_'||temp0[i]==',') //Tracing errors.
        {
            printf("\n");
            printf("Error..\nThere are incorrect characters in the infix expression.\n");
            error();
        }
    for(i=0; i<strlen(temp0); i++)
        if(temp0[i]=='+'&&temp0[i+1]=='-'&&temp0[i+2]=='-'||temp0[i]=='-'&&temp0[i+1]=='-'&&temp0[i+2]=='-'||temp0[i]=='*'&&temp0[i+1]=='-'&&temp0[i+2]=='-'||temp0[i]=='/'&&temp0[i+1]=='-'&&temp0[i+2]=='-'||temp0[i]=='-'&&temp0[i+1]=='-'&&temp0[i+2]=='+'||temp0[i]=='-'&&temp0[i+1]=='-'&&temp0[i+2]=='*'||temp0[i]=='+'&&temp0[i+1]=='-'&&temp0[i+2]=='/')
        {
            printf("\n");
            printf("Error..\nThe infix expression is incorrect.\n");
            error();
        }
    i=0;
    if(temp0[0]=='-') //To correctly evaluate negative numbers.
    {
        infix[j]='_';
        j++;
        i++;
    }
    while(temp0[i]!='\0')
    {
        infix[j]=temp0[i];
        if(temp0[i]=='+'&&temp0[i+1]=='-'||temp0[i]=='*'&&temp0[i+1]=='-'||temp0[i]=='/'&&temp0[i+1]=='-'||temp0[i]=='-'&&temp0[i+1]=='-'||temp0[i]=='('&&temp0[i+1]=='-') //To correctly evaluate negative numbers.
        {
            j++;
            infix[j]='_';
            i++;
        }
        j++;
        i++;
    }
    infix[j]='\0';
    i=0;
    j=0;

    for(i=0; i<strlen(infix); i++) //Conversion of infix into Postfix expression.
    {
        if (infix[i]==' ')
            continue; //Disregarding spaces
        else if (isdigit(infix[i])||infix[i]=='_')
        {
            if(infix[i]=='_')
            {
                postfix[j]='_';
                i++;
                j++;
            }
            while(isdigit(infix[i]))
            {
                postfix[j]=infix[i];
                j++;
                i++;
            }
            if(infix[i]!='.')
            {
                postfix[j]=',';
                j++;
            }
            if(infix[i]=='.')
            {
                postfix[j]='.';
                j++;
                i++;
            }
            i--;
        }
        else if(infix[i]==')')
        {
            while(!isEmpty(&s)&&Top(&s)!='(') //Keep popping till the opening bracket.
            {
                postfix[j]=pop(&s);
                j++;
            }
            if (isEmpty(&s))  //Tracing errors.
            {
                printf("\n");
                printf("Error..\nThe infix expression is incomplete.\n");
                error();
            }
            else
                pop(&s); //Pop the opening bracket.
        }
        else if (isEmpty(&s))
            push(&s,infix[i]);
        else
        {
            while(!isEmpty(&s)&&Priority(Top(&s))>=Priority(infix[i])&&Top(&s)!='(') //To correctly evaluate it.
            {
                postfix[j]=pop(&s); //Keep popping.
                j++;
            }
            push(&s,infix[i]); //Push the new operator.
        }
    }
    while(!isEmpty(&s))
    {
        postfix[j]=pop(&s); //Keep popping the remaining operators.
        j++;
    }
    postfix[j]='\0';
    for(i=0; i<strlen(postfix); i++) //Storing the postfix expression in the array of structure.
    {
        if(postfix[i]==',')
        {
            arexp[n].pexp[q]=' ';
            q++;
        }
        else
        {
            if(postfix[i]=='_')
            {
                arexp[n].pexp[q]='-';
                q++;
            }
            else if (postfix[i]=='+'||postfix[i]=='-'||postfix[i]=='*'||postfix[i]=='/')
            {
                arexp[n].pexp[q]=postfix[i];
                q++;
                arexp[n].pexp[q]=' ';
                q++;
            }
            else
            {
                arexp[n].pexp[q]=postfix[i];
                q++;
            }
        }
    }
    arexp[n].pexp[q]='\0';

//End of conversion && Start of evaluation.

    j=0;
    char temp1[60];
    for (i=0; i<strlen(postfix); i++)
    {
        if(postfix[i] == ' ') //Disregard spaces.
            continue;
        if (postfix[i]<='9' && postfix[i]>='0'||postfix[i]=='_')
        {
            total=0;
            while(postfix[i]!=',')
            {
                if(postfix[i]=='_')
                {
                    temp1[j]='-';
                    j++;
                    i++;
                }
                temp1[j] = postfix[i];
                i++;
                j++;
            }
            temp1[j]='\0';
            total=atof(temp1); //Change the type of total into float.
            push(&s,total);
            j=0;
        }
        else
        {
            if (!isEmpty(&s))  //Tracing errors.
                a=pop(&s);
            else
            {
                printf("\n");
                printf("Error..\nThe infix expression is incorrect.\n");
                error();
            }

            if (!isEmpty(&s))  //Tracing errors.
                b=pop(&s);
            else
            {
                printf("\n");
                printf("Error..\nThe infix expression is incomplete.\n");
                error();
            }

            switch(postfix[i])
            {
            case '+':
                push(&s,b+a);
                break;
            case '-':
                push(&s,b-a);
                break;
            case '*':
                push(&s,b*a);
                break;
            case '/':
                if (a==0)  //Tracing errors.
                {
                    printf("\n");
                    printf("Error..\nThe infix expression in mathematically incorrect.\n");
                    printf("(You can't divide any number by 0).\n");
                    error();
                }
                else
                    push(&s,b/a);
                break;
            default :
                printf("\n");
                printf("Error..\nThere are incorrect characters in the infix expression.\n");
                printf("The only available operations are +, -, * & /.\n");
                error();
            }
        }
    }
    if (!isEmpty(&s))  //Tracing errors.
        x=pop(&s);
    else
    {
        printf("\n");
        printf("Error..\nThe infix expression is incorrect.\n");
        error();
    }

    if (isEmpty(&s))
        return x;
    else  //Tracing errors.
    {
        printf("\n");
        printf("Error..\nThe infix expression is incomplete.\n");
        error();
    }
}

void Check_Repeated_Name (char infix[])
{
    int i,i2;
    int f=0;
    for (i=0; i<n; i++)
        if (strcmp(arexp[i].var,arexp[n].var)==0)
        {
            f=1; //A flag to check the repetition of the expression name.
            i2=i; //The index of the repeated name in the array.
        }
    if (f==1&&r==0)
    {   r=1;
        printf("\n");
        printf("Expression name already exists.\n\n");
        printf("Press (1) to modify.\n");
        printf("Press (2) to add another variable.\n");
        printf("Press (3) to return to main menu.\n");
        switch (getch())
        {
        case '1' : //Swap expression's data.
            strcpy(arexp[i2].result,arexp[n].result);
            strcpy(arexp[i2].mexp,arexp[n].mexp);
            strcpy(arexp[i2].pexp,arexp[n].pexp);
            n--; //Remove the repeated expression.
            printf("\n");
            printf("Expression has been modified successfully.\n");
            Redirect();
            break;
        case '2' :
            n--; //Remove the repeated expression.
            printf("\n");
            Add();
            break;
        case '3':
            Main_Menu();
        default:
            n--; //Remove the repeated expression.
            printf("\n");
            printf("Incorrect entry.\n");
            Redirect();
        }
    }
    else if(r==1)
    {
        r=0;
        printf("\n");
        printf("Expression name already exists.\n");
        Redirect();
    }
}

void Delete_All()
{

    n=-1; //Remove all existing expressions.
    printf("All expressions have been removed successfully.\n");
    printf("\n");
    Redirect();
}

void Delete_Expression()
{
    char name[20];
    int i,i2;
    int f=0;
    exp Delete_exp;
    printf("Enter the expression name you want to delete : ");
    gets(name);
    if(strlen(name)==0)
    {
        printf("\n");
        printf("No entry found.\n");
        printf("Press any key to return to main menu.\n");
        getch();
        Main_Menu();
    }
    for (i=0; i<=n; i++)
        if(strcmp(name,arexp[i].var)==0)
        {
            f=1; //A flag to check if the variable name exists.
            i2=i; //the index of the variable.
        }
    if (f==0)
    {
        printf("\n");
        printf("The entered expression doesn't exist.\n");
        printf("\n");
        Redirect();
    }
    else
    {
        for (i=i2; i<=n; i++) //Relocating the expressions.
        {
            strcpy(arexp[i].var,arexp[i+1].var);
            strcpy(arexp[i].mexp,arexp[i+1].mexp);
            strcpy(arexp[i].pexp,arexp[i+1].pexp);
            strcpy(arexp[i].result,arexp[i+1].result);
        }
        n--; //Deleting the expression.
        printf("\n");
        printf("Expression has been removed successfully.\n");
        Redirect();
    }
}

void Print_All()
{
    int i;
    for(i=0; i<=n; i++) //Printing all expressions.
    {
        printf("[infix]   %s = %s\n",arexp[i].var,arexp[i].mexp);
        printf("[postfix] %s = %s\n",arexp[i].var,arexp[i].pexp);
        printf("[result]  %s = %s\n",arexp[i].var,arexp[i].result);
        printf("\n");
    }
    printf("\n");
    Redirect();
}

void Print_Average()
{
    float average=0;
    float total;
    int i;
    for(i=0; i<=n; i++) //Calculate the sum of the expressions.
    {
        total=atof(arexp[i].result); //Change the type of the results into float.
        average =average + total;
    }
    average=average/(n+1);  //Calculate the average of the expressions.
    printf("[Average] = %.2f\n",average);
    printf("\n");
    Redirect();
}

void Print_Sum()
{
    float Sum=0;
    float total;
    int i;
    for(i=0; i<=n; i++) //Calculate the sum of the expressions.
    {
        total=atof(arexp[i].result); //Change the type of the results into float.
        Sum =Sum + total;
    }
    printf("[Sum] = %.2f\n",Sum);
    printf("\n");
    Redirect();
}

void Print_Max_Min()
{
    float temp[n];
    int i;
    int i1,i2;
    for(i=0; i<=n; i++) //Change the type of the results into float.
    {
        temp[i]=atof(arexp[i].result);
    }
    float Max=temp[0];
    float Min=temp[0];
    for(i=0; i<=n; i++)
    {
        if (temp[i]>=Max)
        {
            Max=temp[i];
            i1=i; //Index of the expression of the maximum value.
        }
        if (temp[i]<=Min)
        {
            Min=temp[i];
            i2=i; //Index of the expression of the minimum value.
        }
    }
    printf("[Maximum] %s = %.2f \n",arexp[i1].var,Max);
    printf("[Minimum] %s = %.2f \n",arexp[i2].var,Min);
    printf("\n");
    Redirect();
}

void Print_Sorted()
{
    int q=1,a=0;
    exp temp1; //Temporary for swapping.
    exp Sarexp[20]; //New sorted array of structure.
    float temp[n]; //Stores the expressions' results after conversion.
    float tempx; //Temporary for swapping.
    for(a=0; a<=n; a++) //Change the type of the results into float.
    {
        temp[a]=atof(arexp[a].result);
    }

    for (a=0; a<=n; a++) //Copy all the expressions to the new sorted array of structure.
    {
        strcpy(Sarexp[a].var,arexp[a].var);
        strcpy(Sarexp[a].mexp,arexp[a].mexp);
        strcpy(Sarexp[a].pexp,arexp[a].pexp);
        strcpy(Sarexp[a].result,arexp[a].result);
    }

    for (a=0; a<=n; a++) //Swapping where the lower value gets a lower index.
    {
        for (q=a+1; q<n+1; q++)
            if (temp[a]>temp[q])
            {
                strcpy(temp1.var,Sarexp[a].var);
                strcpy(Sarexp[a].var,Sarexp[q].var);
                strcpy(Sarexp[q].var,temp1.var);

                strcpy(temp1.mexp,Sarexp[a].mexp);
                strcpy(Sarexp[a].mexp,Sarexp[q].mexp);
                strcpy(Sarexp[q].mexp,temp1.mexp);

                strcpy(temp1.pexp,Sarexp[a].pexp);
                strcpy(Sarexp[a].pexp,Sarexp[q].pexp);
                strcpy(Sarexp[q].pexp,temp1.pexp);

                strcpy(temp1.result,Sarexp[a].result);
                strcpy(Sarexp[a].result,Sarexp[q].result);
                strcpy(Sarexp[q].result,temp1.result);

                tempx=temp[a];
                temp[a]=temp[q];
                temp[q]=tempx;
            }
    }

    for(a=0; a<=n; a++) //Printing all available expressions after sorting.
    {
        printf("[infix]   %s = %s\n",Sarexp[a].var,Sarexp[a].mexp);
        printf("[postfix] %s = %s\n",Sarexp[a].var,Sarexp[a].pexp);
        printf("[result]  %s = %s\n",Sarexp[a].var,Sarexp[a].result);
        printf("\n");
    }

    printf("\n");
    Redirect();
}

void Print_Number()
{
    int x=n+1; //The number of saved expressions.
    printf("The number of available expressions is : %d\n",x);
    printf("\n");
    Redirect();
}

void Modify()
{
    if (n==-1) //No expressions available.
    {
        printf("There are no variables to modify.\n");
        printf("\n");
        Redirect();
    }
    int i,i2=0,f=0;
    char name[20]; //Old expression name.
    char New[20]; //New expression name.
    printf("Enter the variable name you want to modify : ");
    gets(name);
    if(strlen(name)==0)
    {
        printf("\n");
        printf("No entry found.\n");
        printf("Press any key to return to main menu.\n");
        getch();
        Main_Menu();
    }
    for (i=0; i<=n; i++) //Check if the expression name exists.
        if(strcmp(name,arexp[i].var)==0)
            i2=i+1; //The index of the expression name +1.
    if (i2==0)
    {
        printf("\n");
        printf("Expression name doesn't exist.\n");
        printf("\n");
        Redirect();
    }

    printf("\n");
    printf("Enter the new variable name : ");
    gets(New);
    if(strlen(New)==0)
    {
        printf("\n");
        printf("No entry found.\n");
        printf("Press any key to return to main menu.\n");
        getch();
        Main_Menu();
    }
    for(i=0; i<strlen(New); i++) //Tracing errors.
        if(isalpha(New[i]))
            f=1;

    if (f==0)
    {
        printf("\n");
        printf("Error..\nThe variable name must contain an alphabet.\n");
        Redirect();
    }

    for(i=0; i<strlen(New); i++) //Tracing errors.
        if(New[i]=='+'||New[i]=='-'||New[i]=='*'||New[i]=='/'||New[i]=='('||New[i]==')')
        {
            printf("\n");
            printf("Error..\nThe variable name can't contain spaces , parentheses \nor the operators characters.\n");
            printf("(i.e. +, -, * & /).\n");
            printf("\n");
            Redirect();
        }

    for (i=0; i<=n; i++)  //Tracing errors.
        if (strcmp(New,arexp[i].var)==0)
        {
            printf("\n");
            printf("Error..\nExpression name already exists.\n");
            Redirect();
        }

    strcpy(arexp[i2-1].var,New); //Swapping names.
    printf("\n");
    printf("Variable name has been modified successfully.\n");
    Redirect();
}

void Save_File()
{
    if (n==-1) //No expressions available.
        printf("There are no expressions to save.\n\n");
    else
    {
        int j=0;
        p=fopen("Project.txt","w");
        fprintf(p,"%d\n",n); //Saving the number of expressions.
        for(j=0; j<=n; j++) //saving expressions' data.
        {
            fprintf(p,"%s _ %s _ %s _ %s\n",arexp[j].var,arexp[j].mexp,arexp[j].pexp,arexp[j].result);
        }
        fclose(p);
        if (n==0) //:D
            printf("%d expression has been save successfully.\n\n",n+1);
        else
            printf("%d expressions have been saved successfully.\n\n",n+1);
    }
}

void Load_File()
{
    int i=0;
    int x;
    p=fopen("Project.txt","r");
    if(p==0) //Check if the file exists.
    {
        printf("File not found.\n\n");
        printf("Press (1) to create a new file.\n");
        printf("Press (2) to continue to main menu.\n\n");
        switch(getch())
        {
        case '1':
            Create_File();
            break;
        case '2':
            Main_Menu();
            break;
        default:
            Redirect();
        }
    }
    fseek(p, 0, SEEK_END);
    x = ftell(p); //Size of the file.
    fclose(p);
    if (x==0) //Check if the file is empty.
    {
        printf("There are no Expressions to load.\n\n");
        Redirect();
    }
    else
    {
        p=fopen("Project.txt","r");

        if(!feof(p)) //Loading the number of expressions.
        {
            fscanf(p,"%d",&n);
            fscanf(p,"\n");
        }

        while(!feof(p)) //Loading the expressions' data.
        {
            fscanf(p,"%s _%[^_] _ %[^_] _ %[^\n]",arexp[i].var,arexp[i].mexp,arexp[i].pexp,arexp[i].result);
            fscanf(p,"\n");
            i++;
        }
    }
    fclose(p);
    if (n==0) //:D
        printf("%d expression has been loaded successfully.\n\n",n+1);
    else
        printf("%d expressions have been loaded successfully.\n\n",n+1);
    Redirect();
}

void Reset_File()
{
    int x;
    p=fopen("Project.txt","r");
    fseek(p, 0L, SEEK_END);
    x = ftell(p); //Size of the file.
    if (p==NULL) //Check if the file exists.
    {
        printf("File doesn't exist.\n\n");
        Redirect();
    }
    else if (x==0) //Check if the file is empty.
    {
        fclose(p);
        printf("There are no expressions to be removed.\n\n");
        Redirect();
    }
    else
    {
        p=fopen("Project.txt","w"); //Remove all stored expressions.
        fclose(p);
        printf("Old expressions have been removed successfully.\n\n");
        Redirect();
    }
}

void Create_File()
{
    p=fopen("Project.txt","r");
    if (p==NULL) //Check if the file exists.
    {
        p=fopen("Project.txt","w"); //Create a new file.
        fclose(p);
        printf("\n");
        printf("File has been created successfully.\n\n");
        Redirect();
    }
    else
    {
        printf("\n");
        printf("File already exists.\n\n");
        Redirect();
    }

}

void Delete_File()
{
    p=fopen("Project.txt","r");
    if (p==NULL) //Check if the file exists.
    {
        printf("File doesn't exist.\n\n");
        Redirect();
    }
    else
    {
        fclose(p);
        remove("Project.txt");
        printf("File has been removed successfully.\n\n");
        Redirect();
    }
}

void Main_Menu()
{
    system("cls");
    printf("\t\t**********WELCOME TO OUR PROJECT*************");
    printf("\n\n\t\t\t\t MAIN MENU\n\n");
    printf("Press (1) to add a new expression.\n");
    printf("Press (2) to modify.\n");
    printf("Press (3) to print.\n");
    printf("Press (4) to remove.\n");
    printf("Press (5) to manage file.\n");
    printf("Press (6) to exit.\n");
    switch(getch())
    {
    case '1':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\t   ADD\n\n");
        Add();
        break;
    case '2':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\t  MODIFY\n\n");
        Modify();
        break;
    case '3' :
        Print_Menu();
        break;
    case '4':
        Delete_Menu();
    case '5':
        Manage_File_Menu();
    case '6':
        Exit_Menu();
    default:
        printf("\n");
        printf("Incorrect entry, please try again.\n");
        Redirect();
    }
}

void Start_Menu()
{
    system("cls");
    printf("\t\t**********WELCOME TO OUR PROJECT*************");
    printf("\n\n\t\t\t\t  START\n\n");
    printf("Press (1) to load old expressions.\n");
    printf("Press (2) to continue to main menu.\n");
    switch(getch())
    {
    case '1':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\t  START\n\n");
        Load_File();
        break;
    case '2':
        Main_Menu();
        break;
    default:
        printf("\n");
        printf("Incorrect entry, please try again.\n");
        printf("Press any key to continue.\n");
        getch();
        main();
    }
}

void Exit_Menu()
{
    system("cls");
    printf("\t\t**********WELCOME TO OUR PROJECT*************");
    printf("\n\n\t\t\t\t  EXIT\n\n");
    printf("Press (1) to save expressions.\n");
    printf("Press (2) to exit without saving.\n");
    printf("Press (3) to return to main menu.\n");
    switch(getch())
    {
    case '1':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\t  EXIT\n\n");
        Save_File();
        printf("Thanks for using our project.\n");
        printf("Press any key to exit.\n");
        getch();
        exit(0);
        break;
    case '2':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\t  EXIT\n\n");
        printf("Thanks for using our project.\n");
        printf("Press any key to exit.\n");
        printf("Press any key to continue.\n");
        getch();
        exit(0);
        break;
    case '3':
        Main_Menu();
        break;
    default :
        printf("\n");
        printf("Incorrect entry, please try again.\n");
        getch();
        Exit_Menu();
    }
}

void Delete_Menu()
{
    system("cls");
    printf("\t\t**********WELCOME TO OUR PROJECT*************");
    printf("\n\n\t\t\t\t  DELETE\n\n");
    if (n==-1) //No expressions available.
    {
        printf("There are no expressions to remove.\n");
        printf("\n");
        Redirect();
    }
    else
    {
        printf("Press (1) to remove an expression.\n");
        printf("Press (2) to remove all expressions.\n");
        printf("Press (3) to return to main menu.\n");
        switch (getch())
        {
        case '1':
            system("cls");
            printf("\t\t**********WELCOME TO OUR PROJECT*************");
            printf("\n\n\t\t\t\t  DELETE\n\n");
            Delete_Expression();
            break;
        case '2':
            system("cls");
            printf("\t\t**********WELCOME TO OUR PROJECT*************");
            printf("\n\n\t\t\t\t  DELETE\n\n");
            Delete_All();
            break;
        case '3':
            Main_Menu();
            break;
        default:
            printf("\n");
            printf("Incorrect entry, please try again.\n");
            printf("Press any key to continue.\n");
            getch();
            Delete_Menu();
        }
    }
}

void Print_Menu()
{
    system("cls");
    printf("\t\t**********WELCOME TO OUR PROJECT*************");
    printf("\n\n\t\t\t\t  PRINT\n\n");
    if (n==-1) //No expressions available.
    {
        printf("There are no expressions to print.\n");
        printf("\n");
        Redirect();
    }
    else
    {
        printf("Press (1) to print all expressions.\n");
        printf("Press (2) to print all expressions after sorting.\n");
        printf("Press (3) to show the number of expressions.\n");
        printf("Press (4) to show the sum of all expressions.\n");
        printf("Press (5) to show the average of all expressions.\n");
        printf("Press (6) to print the maximum and minimum expressions.\n");
        printf("Press (7) to return to main menu.\n");
        switch(getch())
        {
        case '1':
            system("cls");
            printf("\t\t**********WELCOME TO OUR PROJECT*************");
            printf("\n\n\t\t\t\t  PRINT\n\n");
            Print_All();
            break;
        case '2':
            system("cls");
            printf("\t\t**********WELCOME TO OUR PROJECT*************");
            printf("\n\n\t\t\t\t  PRINT\n\n");
            Print_Sorted();
            break;
        case '3':
            system("cls");
            printf("\t\t**********WELCOME TO OUR PROJECT*************");
            printf("\n\n\t\t\t\t  PRINT\n\n");
            Print_Number();
            break;
        case '4':
            system("cls");
            printf("\t\t**********WELCOME TO OUR PROJECT*************");
            printf("\n\n\t\t\t\t  PRINT\n\n");
            Print_Sum();
            break;
        case '5':
            system("cls");
            printf("\t\t**********WELCOME TO OUR PROJECT*************");
            printf("\n\n\t\t\t\t  PRINT\n\n");
            Print_Average();
            break;
        case '6':
            system("cls");
            printf("\t\t**********WELCOME TO OUR PROJECT*************");
            printf("\n\n\t\t\t\t  PRINT\n\n");
            Print_Max_Min();
            break;
        case '7':
            Main_Menu();
            break;
        default:
            printf("\n");
            printf("Incorrect entry, please try again.\n");
            printf("Press any key to continue.\n");
            getch();
            Print_Menu();
        }
    }
}

void Manage_File_Menu()
{
    system("cls");
    printf("\t\t**********WELCOME TO OUR PROJECT*************");
    printf("\n\n\t\t\t\tMANAGE FILE\n\n");
    printf("Press (1) to save.\n");
    printf("Press (2) to load.\n");
    printf("Press (3) to reset.\n");
    printf("Press (5) to create.\n");
    printf("Press (4) to delete.\n");
    printf("Press (6) to return to main menu.\n");
    switch(getch())
    {
    case '1':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\tMANAGE FILE\n\n");
        Save_File();
        Redirect();
        break;
    case '2':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\tMANAGE FILE\n\n");
        Load_File();
        break;
    case '3':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\tMANAGE FILE\n\n");
        Reset_File();
        break;
    case '4':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\tMANAGE FILE\n\n");
        Create_File();
        break;
    case '5':
        system("cls");
        printf("\t\t**********WELCOME TO OUR PROJECT*************");
        printf("\n\n\t\t\t\tMANAGE FILE\n\n");
        Delete_File();
        break;
    case '6':
        Main_Menu();
        break;
    default:
        printf("\n");
        printf("Incorrect entry, please try again.\n");
        printf("Press any key to continue.\n");
        getch();
        Manage_File_Menu();
    }
}
