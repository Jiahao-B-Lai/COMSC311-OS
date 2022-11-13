// Author: Jiahao Lai
// Email: jvl6364@psu.edu
#include <string.h>
#include <stdlib.h>
#include "mystring.h"
int mystrcmp(const char *s1, const char *s2) {
  //set a return value if s1 and s2 are not the same;
  //because strcmp return the difference of ASCII values of 
  //the first unmatched character of two strings; 
  int rvalue;
  //set a counter i here to traverse the string
  int i = 0;
  //start compare each character in two strings
  //to find when does the first unmatched character appear;
  //if two strings are the same, we need to reach to the end of the string
  //and string terminator is needed;
  while ((s1[i] == s2[i]) && (s1[i] != '\0' && s2[i] != '\0')){
    i++;
  }
  //if s1 is smaller or larger than s2
  if (s1[i] != s2[i]){
    rvalue = (s1[i] - s2[i]);
    return rvalue;
  }
  //if s1 == s2
  else 
  return 0;
}

char *mystrdup(const char *s) {
  //if it is empty, return NULL
  if (s == NULL){
    return NULL;
  }
  //create a counter j to count the number of elements in the string
  int j = 0;
  //count the number of elements
  while(s[j] != '\0'){
    j++;
  }
  //give a new memory address to store the copy of the string
  char *new;
  new = malloc(j);
  //create another counter k to traverse the strings
  //and copy the old string to new one
  int k = 0;
  while(s[k] != '\0'){
    new[k] = s[k];
    k++;
  }
  //end with '\0'
  new[k] = '\0';
  return new;
}
