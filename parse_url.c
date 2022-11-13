/*
 * author: Jiahao Lai
 * email: jvl6364@psu.edu
 * parse_url.c for CMPSC 311 Fall 2019
 * last updated: 12/02/2019
 */
#include "parse_url.h"
#include <string.h>
// returns 1 if the url starts with http:// (case insensitive)
// and returns 0 otherwise
// If the url is a proper http scheme url, parse out the three
// components of the url: host, port and path and copy
// the parsed strings to the supplied destinations.
// Remember to terminate your strings with 0, the null terminator.
// If the port is missing in the url, copy "80" to port.
// If the path is missing in the url, copy "/" to path.
int parse_url(const char *url, char *host, char *port, char *path) {
  //check if the url starts with http://
  //if it is, start parsing
  if (strncasecmp(url, "http://", 7) == 0){
    int len = strlen(url);  
    int i;
    int j = 0;
    //check the position of '/' to parse the host
    //here host may include the port 
    for (i = 7; url[i] != '/'; i++){
      if (url[i] == '\0')
        break;
      host[j] = url[i];
      j++;
    }
    //add terminator
    host[j] = '\0';
    //check the position of ':' to parse the port from the host
    int a;
    int b = 0;
    for (a = 0; host[a] != ':'; a++){
      if (host[a] == '\0')
        break;
      b++;
    }
    //if port is missing
    //set port as "80"
    if(b == j){
      strncpy(port, "80", 2);
      port[2] = '\0';
    }
    //if port exists
    //set port after the posistion of ":"
    else{
      int c;
      int d = 0;
      for (c = b + 1; host[c] != '\0'; c++){
        port[d] = host[c];
        d++;
      }
      //add terminator
      port[d] = '\0';
    }
    //pasring path
    //if path is missing
    //set path as "/"
    if((j+7) == len){
      strncpy(path, "/", 1);
      path[1] = '\0';
    }
    //if path exits
    //set path after the position of "/"
    else{
      int e;
      int f = 0;
      for (e = 7+j; url[e] != '\0'; e++){
        path[f] = url[e];
        f++;
      }
      //add terminator
      path[f] = '\0';
    }
    //move the port from the host
    host[b] = '\0';
    return 1;
  }
  //if the url does not start with "http://"
  else
    return 0;
}
