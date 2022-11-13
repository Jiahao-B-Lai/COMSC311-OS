/*
 * author: Jiahao Lai
 * email: jvl6364@psu.edu
 * dict.c for CMPSC 311 Fall 2019
 * last updated: 10/23/2019
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

// count number of keys in a dict.
int countKeys(const dictNode *dict) {
  //set counter count
  int count = 0;
  //Traverse the list and count the nodes
  while (dict != NULL){
    dict = dict->next;
    count++;
  }
  return count;
}

// given a key, look up its corresponding value in the
// dictionary, returns -1 if the value is not in the dictionary.
// your search for key should end when the key in the next node
// is bigger than the lookup key or you reached the end of the
// list.
int lookupKey(const dictNode *dict, const char *key) {
  //Traverse the list
  while(dict != NULL){
    //use strcmp to compare keys
    //set helper temp1 to decide if we find the key;
    int temp1 = strcmp(dict->key,key);
    //find the key!
    if (temp1 == 0)
      return dict->value;
    //not found yet, go to next
    else if (temp1 < 0)
      dict = dict->next;
    //failed to find the key
    else
      return -1;
  }
  return -1;
}

// delete the node in a dict with given key, return the value of
// the deleted node if it was found, return -1 if it wasn't found.
int deleteKey(dictNode **dict, const char *key) {
  //create current node and previous node
  dictNode *current = *dict, *pre = NULL;
  //create findK to help to find the key
  int findK;
  //create return value
  int rvalue;
  //Traverse the list to find the key
  while (current != NULL){
    findK = strcmp(current->key, key);
    if (findK > 0)
      return -1;
    //find the key! break
    else if (findK == 0)
      break;
    pre = current;
    current = current->next;
  }

  //check if it is empty
  if(current == NULL)
    return -1;
  //if it is not empty
  else{
    //check if delete from head
    if (current == *dict){
      //move head to next
      *dict = (*dict)->next;
      rvalue = current->value;
      freeNode(current);
      return rvalue;
    }
    //delete from middle or tail
    else{
      pre->next = current->next;
      rvalue = current->value;
      freeNode(current);
      return rvalue;
    }
  }
  return -1;
}
// given a key/value pair, first lookup the key in the dictionary,
// if it is already there, update the dictionary with the new
// value; if it is not in the dictionary, insert a new node into
// the dictionary, still make sure that the key is in alphabetical
// order.
// IMPORTANT: When creating a new node, make sure you dynamically
// allocate memory to store a copy of the key in the memory. You
// may use strdup function. DO NOT STORE the input key from the 
// argument directly into the node. There is no guarantee that key
// pointer's value will stay the same. 
// YOU MUST KEEP THE ALPHABETICAL ORDER OF THE KEY in the dictionary.
void addKey(dictNode **dict, const char *key, int value) {
  dictNode *current2 = *dict;
  //define findKey to help us to compare keys and order them
  int findKey;
  int findKey2;
  //If it is empty, create a node, then add key and value
  if (current2 == NULL){
    //add a new node to the empty dict
    dictNode *addnode = malloc(sizeof(dictNode));
    *dict = addnode;
    addnode->next = NULL;
    //set the given key and value to it
    addnode->key = strdup(key);
    addnode->value = value;
    return;
  }
  //If adding a node to the head
  //use findKey to help to compare
  findKey = strcmp(current2->key, key);
  //if the key we add is smaller
  if (findKey > 0){
    //add a new node at the head
    dictNode *addnode2 = malloc(sizeof(dictNode));
    addnode2->next = current2;
    *dict = addnode2;
    //set the given key and value to it
    addnode2->key = strdup(key);
    addnode2->value = value;
    return;
  }
  //checking if updating the head
  //if we find the key, just update the value!
  if (findKey == 0){
    current2->value = value;
    return;
  }
  //check if adding node in the middle 
  //and if we should update the value
  //Traverse thelistand find the right location
  while(current2 != NULL && current2->next != NULL){
    //check if the given key is smaller than the next key in the dict
    findKey2 = strcmp(current2->next->key, key);
    //check if updating the value
    if( findKey2 == 0){
        current2->next->value = value;
        return;
    }
    //check if the given key is smaller than the next key
    //check if we should add a node in the middle  
    if (findKey2 > 0 ){
      //add a new node in the middle
      dictNode *addnode3 = malloc(sizeof(dictNode));
      addnode3->next = current2->next;
      current2->next = addnode3;
      //set the key and value to it
      addnode3->key = strdup(key);
      addnode3->value = value;
      return;
    }
    // go to next node
    current2 = current2->next;
  }
  
  //check if adding a node at the tail
  //fial to find the key in the list
  //add a new node at the tail  
  dictNode *addnode4 = malloc(sizeof(dictNode));
  current2->next = addnode4;
  addnode4->next = NULL;
  //set the key and value to it    
  addnode4->key = strdup(key);
  addnode4->value = value;
  return;
}




