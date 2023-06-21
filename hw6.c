#include "hw6.h"

int main(int argc, char *argv[]) {
//checks for input and output files
FILE *fp = fopen(argv[argc-2], "r");
FILE *fp2 = fopen(argv[argc-1], "w");

//checks for duplicate arguments
int dupeS = 0;
int dupeR = 0;
int dupeL = 0;
int dupeW = 0;
for(int i = 0; i < argc; i++){
if(strcmp((argv[i]), "-r") == 0){
dupeR++;
}
else if(strcmp((argv[i]), "-l") == 0)  {
dupeL++;
}
else if(strcmp((argv[i]), "-s") == 0) {
dupeS++;
}
else if(strcmp((argv[i]), "-w") == 0){
dupeW++;
}
}

//checks if s option is missing
int sOptMiss = 0;
int sIndex = 0;
for(int i = 0; i < argc; i++){
if(strcmp(argv[i], "-s") == 0){
sIndex = i;
sOptMiss++;
}
}

//checks if s argument is missing
int sArgMiss = 0;
int nextIndex;
char *result;
char *str = argv[sIndex + 1];
result = strchr(str, '-');
nextIndex = (int)(result - str);
if(nextIndex == 0){
sArgMiss++;
}

//checks if r option is missing
int rOptMiss = 0;
int rIndex = 0;
for(int i = 0; i < argc; i++){
if(strcmp(argv[i], "-r") == 0){
rIndex = i;
rOptMiss++;
}
}

//checks if r argument is missing
int rArgMiss = 0;
int nextIndex2;
char *result2;
char *str2 = argv[rIndex + 1];
result2 = strchr(str2, '-');
nextIndex2 = (int)(result2 - str2);
if(nextIndex2 == 0){
rArgMiss++;
}

//checks if l option is missing
int lOptMiss = 0;
int lIndex = 0;
for(int i = 0; i < argc; i++){
if(strcmp(argv[i], "-l") == 0){
lOptMiss++;
lIndex = i;
}
}

//checks if l arguments are invalid
int invalidL = 0;
char *str3;
char *result3;
char *token1;
char *end;
long num1 = 0;
long num2 = 0;
char *token2;
char *end2;
if(lOptMiss > 0){
str3 = argv[lIndex + 1];
result3 = strchr(str3, ',');
if(result3 == NULL){
invalidL++;
}
else{
char x[2] = ",";
token1 = strtok(str3, x);
if(token1 == NULL){
invalidL++;
}
num1 = strtol(token1, &end, 10);
token2 = strtok(NULL, x);
if(token2 == NULL){
invalidL++;
}
else{
num2 = strtol(token2, &end2, 10);
if((num2 == 0) || (num1 == 0)){
invalidL++;
}
else if(num1 > num2){
invalidL++;
}
}
}
}

//checks if wildcards are invalid
int invalidW = 0;
int wOptMiss = 0;
int wIndex = 0;
int starIndex = 0;
int starIndex2 = 0;
char *wildStr = argv[sIndex + 1];
for(int i = 0; i < argc; i++){
if(strcmp(argv[i], "-w") == 0){
 wOptMiss++;
 wIndex = i;
}
}
if(wOptMiss > 0){
char *starCheck = strchr(wildStr, '*');
if(starCheck == NULL){
 invalidW++;
}
else{
 char *starCheck2 = strrchr(wildStr, '*');
 starIndex = (int)(starCheck - wildStr);
 starIndex2 = (int)(starCheck2 - wildStr);
 if((starIndex == 0) && (starIndex2 == (strlen(wildStr) - 1))){
   invalidW++;;
 }
}
}

//Error code checkers in order
if(argc < 7){
return MISSING_ARGUMENT;
}
else if(dupeS > 1 || dupeR > 1 || dupeL > 1 || dupeW > 1){
return DUPLICATE_ARGUMENT;
}
else if(fp == NULL){
return INPUT_FILE_MISSING;
}
else if(fp2 == NULL){
return OUTPUT_FILE_UNWRITABLE;
}
else if(sOptMiss == 0){
return S_ARGUMENT_MISSING;
}
else if(sArgMiss > 0){
return S_ARGUMENT_MISSING;
}
else if(rOptMiss == 0){
return R_ARGUMENT_MISSING;
}
else if(rArgMiss > 0){
return R_ARGUMENT_MISSING;
}
else if(invalidL > 0){
return L_ARGUMENT_INVALID;
}
else if(invalidW > 0){
return WILDCARD_INVALID;
}
else{
//search and replace begins
//fp points to input fp2 points to output

char temp[MAX_LINE];
int start = num1;
int fin = num2;
int current = 1;
if((num1 == 0) && (num2 == 0)){
 start = 1;
 fin = MAX_LINE;
}

if(wOptMiss == 0){
   char *word = argv[sIndex + 1];
   char *replace = argv[rIndex + 1];
   int wordLen = strlen(word);
   int repLen = strlen(replace);
  
while(!feof(fp)){
   //gets a line from input and stores into temp
    fgets(temp, MAX_LINE ,fp);
   //checks if search word is in temp of file
   char *ptr = strstr(temp, word);
  
   char str[MAX_LINE];
   int curr = ptr - temp;


//if search word is found, then replace it and store in str
if(ptr != NULL && ((current >= start) && (current <= fin))){
     strcpy(str, temp);
     strcpy(curr + str, replace);
     strcpy(curr + str + repLen, ptr + wordLen);
   
     int x = 1;
     ptr = strstr(ptr + wordLen, word);
     while(ptr != NULL){
         curr = ptr - temp + (x * (repLen - wordLen));
         strcpy(curr + str, replace);
         strcpy(curr + str + repLen, ptr + wordLen);
         x++;
         ptr = strstr(ptr + wordLen, word);
     }


     //then put it into the output
     fputs(str, fp2);
     current++;
 
}
   else{ //otherwise just store temp into output
        fputs(temp, fp2);
        current++;
  
   }
 }
}

//wildcard cases
else if(wOptMiss == 1){
   char str[MAX_LINE];
   char *word = argv[sIndex + 1];
   char *replace = argv[rIndex + 1];
   int x = 1;
   char temp[10000];
  
   if(starIndex == 0){ //if star is on the left
       word++;//remove the star
     
    }
    else if(starIndex2 == (strlen(wildStr) - 1)){  //if star is on the right
       word[starIndex2] = '\0';
      
    }
   
    int wordLen = strlen(word);
    int repLen = strlen(replace);
  
  //PREFIX CASES
  if(starIndex != 0){
   while(!feof(fp)){
    fgets(temp, MAX_LINE,fp);
    char *ptr = strstr(temp, word);
    int curr = ptr - temp; //index
    int prevIndex = 0;
    
    if(curr != 0){
     prevIndex = curr - 1;
    }
   
    int spaceCount = 0;
    int validCount = 0;
    int lostSpace = 0;
    
    if(ptr == NULL){ // if word is not in line, just print it
     fputs(temp, fp2);
     current++;
     }
     else if(ptr != NULL && (current < start)){
         fputs(temp, fp2);
         current++;
     }
     else if(ptr != NULL && (current > fin)){
          fputs(temp, fp2);
         current++;
     }
   
    //if the word is found in a line
     while(ptr != NULL && ((current >= start) && (current <= fin))){
       
         //check if the word is a valid word
        if(starIndex != 0 && ((isspace(temp[prevIndex]) != 0) || ( ispunct(temp[prevIndex]) != 0)) || curr == 0){
            validCount++;
            //if word is valid then replace it
            if(validCount == 1){ //just for first word
             strcpy(str, temp);
            
             spaceCount = curr;
             while((isspace(temp[spaceCount]) == 0)){
                 spaceCount++;
                 if(temp[spaceCount] == NULL){
                     
                     break;
                 }
             }
              if(ispunct(temp[spaceCount - 1]) != 0){
                     spaceCount = spaceCount - 1;
              }
             spaceCount = spaceCount - curr;
             lostSpace = spaceCount - repLen;
             strcpy(curr + str, replace);
             strcpy(curr + str + repLen, ptr + spaceCount);
              ptr = strstr(ptr + spaceCount, word);
               curr = ptr - temp;
               prevIndex = curr - 1;
               x = 1;
            
            }
       
         //if another word is not found, put str into output
         if(ptr == NULL){
            fputs(str, fp2);
         }
         else{
             //if another word is found in line
           
             if(starIndex != 0 && ((isspace(temp[prevIndex]) != 0) || ( ispunct(temp[prevIndex]) != 0)) || curr == 0){
                 
                 curr = ptr - temp - lostSpace;
                 strcpy(curr + str, replace);
                
                 int newCurr = ptr - temp;
                 spaceCount = newCurr;
                 
                 while((isspace(temp[spaceCount]) == 0)){
                     spaceCount++;
                     if(temp[spaceCount] == NULL){
                     
                     break;
                 }
                 }
                 if(ispunct(temp[spaceCount - 1]) != 0){
                         spaceCount = spaceCount - 1;
                 }
                
                 spaceCount = spaceCount - newCurr;
                 lostSpace = lostSpace + (spaceCount - repLen);
                 strcpy(curr + str + repLen, ptr + spaceCount);
                 x++;
                 
                 ptr = strstr(ptr + spaceCount, word);
                 if(ptr == NULL){
                     fputs(str, fp2);
                 }
        
             }
            
        }
        }
        else{ //if the word is not valid
             ptr = strstr(ptr + wordLen, word);
             if(validCount == 1 && ptr == NULL){
                 fputs(str,fp2);
             }
             else if(validCount != 1 && ptr == NULL){
               fputs(temp, fp2);
               current++;
              }
             curr = ptr - temp; //index
             prevIndex = curr - 1;

        }
     }
   }
}

//SUFFIX CASES
else if (starIndex == 0){
    
    while(!feof(fp)){
    fgets(temp, MAX_LINE,fp);
    char *ptr = strstr(temp, word);
    int curr = ptr - temp; //index
    int nextIndex = curr + wordLen;
    int spaceCount = 0;
    int removeCount = 0;
    int subStar = 0;
    int validCount = 0;
    int lostSpace = 0;
   
  
    if(ptr == NULL){
        fputs(temp, fp2);
        current++;
    }
    else if(ptr != NULL && (current < start)){
        fputs(temp, fp2);
        current++;
    }
    else if(ptr != NULL && (current > fin)){
        fputs(temp, fp2);
        current++;
    }
    
      while(ptr != NULL && ((current >= start) && (current <= fin))){
          if(starIndex == 0 && ((isspace(temp[nextIndex]) != 0) || (ispunct(temp[nextIndex]) != 0)) || (temp[nextIndex] == NULL)){
              validCount++;
              
              if(validCount == 1){
                  strcpy(str, temp);
                  
                  spaceCount = curr;
                
                  while((isspace(str[spaceCount]) == 0) && ispunct(str[spaceCount]) == 0){
                      spaceCount--;
                      removeCount++;
                      if(spaceCount < 0){
                        break;
                      }
                      if(str[spaceCount] == NULL){
                          break;
                      }
                  
                  }
                
                  subStar = spaceCount + 1;
                  removeCount = removeCount - 1;
               
                  
                  lostSpace = repLen - (wordLen + removeCount);
                  strcpy(subStar + str, replace);
                  strcpy(subStar + str + repLen, ptr + wordLen);
                  ptr = strstr(ptr + wordLen, word);
                  curr = ptr - temp;
                  nextIndex = curr + wordLen;
                  x = 1;
                  removeCount = 0;    
                  
              }
              
              if(ptr == NULL){
                  fputs(str, fp2);
              }
              else{
                  if(starIndex == 0 && ((isspace(temp[nextIndex]) != 0) || (ispunct(temp[nextIndex]) != 0)) || (temp[nextIndex] == NULL)){
                      curr = ptr - temp;
                      spaceCount = curr;

                      
                        while((isspace(temp[spaceCount]) == 0) && ispunct(temp[spaceCount]) == 0){
                           spaceCount--;
                           removeCount++;
                           if(temp[spaceCount] == NULL){
                          break;
                      }
                        
                        }
                      
                      subStar = spaceCount + 1;
                      removeCount = removeCount - 1;
                      
                    
                       subStar = subStar + lostSpace;
                       strcpy(subStar + str, replace);
                       
                      lostSpace = lostSpace + (repLen - (wordLen + removeCount));
                      strcpy(subStar + str + repLen, ptr + wordLen);
                      x++;
                      removeCount = 0;
                     
                      ptr = strstr(ptr + wordLen, word);
                      if(ptr == NULL){
                          fputs(str, fp2);
                      }
                  }
              }
          }
          else{
              ptr = strstr(ptr + wordLen, word);
              if(validCount == 1 && ptr == NULL){
                  fputs(str, fp2);
              }
              else if(validCount != 1 && ptr == NULL){
                  fputs(temp, fp2);
                  current++;
              }
              curr = ptr - temp;
              nextIndex = curr + wordLen;
          }
          
      } //inner while
    
    }//while loop
}
fclose(fp);
fclose(fp2);
}
}
return 0;
}

