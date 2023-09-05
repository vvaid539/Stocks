#include <stdio.h>
#include <string.h>
#include "hashset.h"

int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }
  //Provides all the possible commands that can be given
  printf("Hashset Application\n");
  printf("Commands:\n");
  printf("  hashcode <elem>  : prints out the numeric hash code for the given key (does not change the hash set)\n");
  printf("  contains <elem>  : prints the value associated with the given element or NOT PRESENT\n");
  printf("  add <elem>       : inserts the given element into the hash set, reports existing element\n");
  printf("  print            : prints all elements in the hash set in the order they were addded\n");
  printf("  structure        : prints detailed structure of the hash set\n");
  printf("  clear            : reinitializes hash set to be empty with default size\n");
  printf("  save <file>      : writes the contents of the hash set to the given file\n");
  printf("  load <file>      : clears the current hash set and loads the one in the given file\n");
  printf("  next_prime <int> : if <int> is prime, prints it, otherwise finds the next prime and prints it\n");
  printf("  expand           : expands memory size of hash set to reduce its load factor\n");
  printf("  quit             : exit the program\n");
  
  char cmd[128];
  hashset_t hash;
  int success;
  hashset_init(&hash, HASHSET_DEFAULT_TABLE_SIZE);

  while(1){
    printf("HS>> ");                 // print prompt
    success = fscanf(stdin,"%s",cmd);
    if(success==EOF){
      printf("\n");                   // found end of input                        
      break;
    }
    //Understand the quit command and break the while loop
    if(strcmp("quit", cmd)==0 ){     
      if(echo){
        printf("quit\n");
      }
      break;                          
    }
    //Understand the hashcode command and uses hashcode() function to return hashcode
    else if(strcmp("hashcode", cmd)==0 ){ // insertion
      fscanf(stdin,"%s",cmd);
      if(echo){
        printf("hashcode %s\n", cmd);
      }
      printf("%i\n", hashcode(cmd));
    }
    //Understand the contains command and uses the contains() function to determine whether an element is found or not
    else if(strcmp("contains", cmd)==0 ){
      fscanf(stdin, "%s", cmd);
      if(echo){
        printf("contains %s\n", cmd); 
      }
      if(hashset_contains(&hash, cmd)){
        printf("FOUND: %s\n", cmd);
      }
      else{
        printf("NOT PRESENT\n");
      }
    }
    //Understand the add command and uses the add() function to either add an element or state that it already exists
    else if(strcmp("add", cmd)==0){
      fscanf(stdin,"%s",cmd);  
      if(echo){
        printf("add %s\n", cmd);
      }
      if(hashset_add(&hash, cmd) == 0){
        printf("Elem already present, no changes made\n");
      }
    }
    //Understand the print command and print the elements using the hashset_write_elems_ordered() function
    else if(strcmp("print", cmd)==0){
      if(echo){
        printf("print \n");
      }
      hashset_write_elems_ordered(&hash, stdout);
    }
    //Understand the structure command and shows the structure of the hashset using hashset_show_structure()
    else if(strcmp("structure", cmd)==0){
      if(echo){
        printf("structure \n");
      }
      hashset_show_structure(&hash);
    }
    //Understand the clear command and free the fields of the hashset using hashset_free_fields and then intiialize them using hashset_init()
    else if(strcmp("clear", cmd)==0){
      if(echo){
        printf("clear \n");
      }
      hashset_free_fields(&hash);
      hashset_init(&hash, HASHSET_DEFAULT_TABLE_SIZE);
    }
    //Understand the save command to run the hashset_save()
    else if(strcmp("save", cmd)==0){
      fscanf(stdin,"%s",cmd);  
      if(echo){
        printf("save %s \n", cmd);
      }
      hashset_save(&hash, cmd);
    }
    //Understand the load command to call hashset_load() and determine whether the load was successful or not
    else if(strcmp("load", cmd) == 0){
      fscanf(stdin,"%s",cmd);  
      if(echo){
        printf("load %s\n", cmd);
      }
      if(hashset_load(&hash, cmd) == 0){
        printf("load failed\n");
      }
    }
    //Understand the next_prime command to run the next_prime() function to find the prime number
    else if(strcmp("next_prime", cmd) == 0){
      int i = 0;
      fscanf(stdin, "%d", &i);
      if(echo){
        printf("next_prime %d\n", i);
      }
      printf("%i\n", next_prime(i));
    }
    //Understand the expand command and run the hashset_expand function to change the hash
    else if(strcmp("expand", cmd) == 0){
      if(echo){
        printf("expand \n");
      }
      hashset_expand(&hash);
    }
    //A contingency case to check if a command not defined earlier is inputted
    else{                            
      if(echo){
        printf("%s\n",cmd);
      }
      printf("unknown command %s\n", cmd);
    }
  }  

  // end main while loop and free the hashet
  hashset_free_fields(&hash);                     
  return 0;

}
