#include <stdlib.h>
#include <stdio.h>

#define SUCCESS 0
#define FAILED 1

#define ASCII_LINE_BREAK 10

// Auxiliar Customized Stack Data Type and its functions
typedef struct itemType* itemPointer;
typedef struct itemType {
  char readChar;
  itemPointer next;
} itemType;
typedef struct {
  itemPointer bottom, top;
} stackType;

// Initialize the Stack
void startStack(stackType* charStackAddr){
  charStackAddr->top = NULL;
  charStackAddr->bottom = NULL;
}

// Check if the stack is empty
int StackEmpty(stackType stack){
  return ((stack.top == NULL) && (stack.bottom == NULL));
}

// Push an element into the stack
void push(char ch, stackType* charStackAddr){

  itemPointer aux;

  if(StackEmpty(*charStackAddr)){
    charStackAddr->bottom = (itemPointer) malloc(sizeof(itemType));
    charStackAddr->bottom->next = NULL;
    charStackAddr->bottom->readChar = ch;
    charStackAddr->top = charStackAddr->bottom;
    return;
  }

  aux = (itemPointer) malloc(sizeof(itemType));
  aux->readChar = ch;
  aux->next = charStackAddr->top;
  charStackAddr->top = aux;
}

// Remove and element from the filled stack
char pop(stackType* charStackAddr){
  itemPointer aux;
  char data;

  // Special treatment for the case one only element
  if(charStackAddr->top == charStackAddr->bottom){
    aux = charStackAddr->top;
    data = aux->readChar;
    charStackAddr->top = NULL;
    charStackAddr->bottom = NULL;
    free(aux);
    return data;
  }

  aux = charStackAddr->top;
  data = aux->readChar;
  charStackAddr->top = aux->next;
  free(aux);
  return data;
}

// Simple tail implementation: functionality ready, but
// for production it can be optimized in some details, as
// for example displaying error messages in a more sofisticated way,
// packaging the code and installing, etc.

int main(int argc, char** argv)
{
  FILE* fp;
  char c;
  stackType charStack;
  int numberOfLines;
  int beginLine;
  int positionShift;
  
  // Read the input arguments
  // Usage: ./ctail FILE_PATH NUMBER_OF_LINES
  if(argc < 3){
    printf("Error: Please provide the file path and the number of lines.\n");
    return FAILED;
  }

  fp = fopen(argv[1], "r");
  if(fp == NULL){
    printf("Error: Please provide a valid file path.\n");
    return FAILED;
  }

  numberOfLines = atoi(argv[2]);
  if(numberOfLines <= 0){
    printf("Error: Please provide a valid number of lines.\n");
    return FAILED;
  } 

  startStack(&charStack);

  // Read the file backwards until reading NUMBER_OF_LINES lines
  // skip EOF character, start from the real last
  positionShift = -1;
  fseek(fp, positionShift, SEEK_END);
  beginLine = 0;

  while (ftell(fp) >= 0){
    c = (char) fgetc(fp);
    if(c == ASCII_LINE_BREAK){
      beginLine++;
      if (beginLine > numberOfLines)
        break;
    }
    push(c, &charStack);
    positionShift--;
    fseek(fp, positionShift, SEEK_END);
  }

  if(fclose(fp))
    printf("Warning: The file could not be closed with success.\n");

  // Flush the Stack read in stdout
  if(StackEmpty(charStack))
    return SUCCESS;

  while(!StackEmpty(charStack))
    printf("%c",pop(&charStack));

  return SUCCESS;
}
