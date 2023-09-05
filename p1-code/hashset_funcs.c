#include "hashset.h"
#include <string.h>
#include <stdlib.h>
// hashset_funcs.c: utility functions for operating on hash sets. Most
// functions are used in the hashset_main.c which provides an
// application to work with the functions.

/*
Compute a simple hash code for the given character
*/
int hashcode(char key[]){
  int hc = 0;
  for(int i=0; key[i]!='\0'; i++){
    hc = hc*31 + key[i];
  }
  return hc;
}

/*
Initialize the hash set 'hs' to have given size and elem_count
0.
*/
void hashset_init(hashset_t *hs, int table_size){
  hs->elem_count = 0; 
  hs->table_size = table_size;
  //Allocates space to the heap for the array of pointers
  hs->table = malloc(sizeof(hashnode_t *) * table_size); 
  //Sets every value of the pointer array to NULL
  for(int i = 0; i < table_size; i++){
    hs->table[i] = NULL;
  }
  hs->order_first = NULL;
  hs->order_last = NULL;
}

/*
Returns 1 if the parameter `elem` is in the hash set and otherwise. Uses
*/
int hashset_contains(hashset_t *hs, char elem[]){ 
  int index = hashcode(elem) % hs->table_size;
  //If the hashcode given is negative, multiply by -1 to get an valid array index
  if(index <= 0){
    index = index * -1;
  }
  hashnode_t *temp = hs->table[index];
  //Loops through the linked kist to see if the elem is already there
  while(temp != NULL){
    //Compares if the hashnode already exists in the linked list
    if(strcmp(temp->elem, elem) == 0){
      return 1;
    }
    temp = temp->table_next;
  }
  //if not already present, returns 0
  return 0;
}

/*
Adding elems at the front of its associated bucket list
*/
int hashset_add(hashset_t *hs, char elem[]){
  //If the elem already exists, do nothing
  if(hashset_contains(hs, elem)){
    return 0;
  }
  //Allocates space to the heap for the node
  hashnode_t *newNode = malloc(sizeof(hashnode_t));
  //If the hashcode given is negative, multiply by -1 to get an valid array index
  int bucket = hashcode(elem) % hs->table_size;
  if(bucket <= 0){
    bucket = bucket * -1;
  }
  //Copys the elem parameter into the new node created
  strcpy(newNode->elem, elem);
  //Checks if the bucket is empty to determine the data field table_next value
  if(hs->table[bucket] == NULL){
    newNode->table_next = NULL;
  }
  else{
    newNode->table_next = hs->table[bucket];
  }
  newNode->order_next = NULL;
  //If the order_last data field is null, set it to the new node
  if (hs->order_last != NULL) {
    hs->order_last->order_next = newNode;
  }
  //If the order_first data field is null, set it to the new node
  if(hs->order_first == NULL){
    hs->order_first = newNode;
  }
  hs->elem_count++;
  hs->order_last = newNode;
  hs->table[bucket] = newNode;
  return 1;
}

/*
De-allocates nodes/table for `hs`.
*/
void hashset_free_fields(hashset_t *hs){
  //Creates fake nodes to free all the nodes of the ordered list
  hashnode_t* node1 = hs->order_first;
  hashnode_t* node2;
  //Loops through to remove the nodes one at a time
  while(node1 != NULL){
     node2 = node1->order_next;
     free(node1);
     node1 = node2;
  }
  //Free the table that was malloc'ed initially 
  free(hs->table);
  //Sets the data_fields to NULL and 0 as appropriate
  hs->table = NULL;
  hs->table_size = 0;
  hs->order_first = NULL;
  hs->order_last = NULL;
  hs->elem_count = 0;
}

/*
Displays detailed structure of the hash set.
*/
void hashset_show_structure(hashset_t *hs){
  //Prints out the elem_count and table_size data fields 
  printf("elem_count: %i\n", hs->elem_count);
  printf("table_size: %i\n", hs->table_size);
  //Determines that if the first node if the ordered array of nodes is NULL and print NULL if the statement is true and the node->elem of the first node if not
  if(hs->order_first->elem != NULL){
    printf("order_first: %s\n", hs->order_first->elem);
  }
  else{
    printf("order_first: NULL\n");
  }
  // Determines that if the last nodeif the ordered array of nodes and print NULL if the statement is true and the node->elem of the last node if not
  if(hs->order_last->elem != NULL){
    printf("order_last : %s\n", hs->order_last->elem);
  }
  else{
    printf("order_last : NULL\n");
  }
  //Calculates and prints the load factor
  double load_factor = (double) hs->elem_count / hs->table_size;
  printf("load_factor: %.4f\n", load_factor);
  //Iterates through each bucket and linked list associated with the bucket in the table
  for(int i = 0; i < hs->table_size; i++){
    printf("[%2d] : ", i);
    hashnode_t* node = hs->table[i];
    //Prints the the data fields of the node, so long as the node does not equal to null for a linked list located in bucket i
    while (node != NULL) {
      if(node->order_next != NULL){
        printf("{%ld %s >>%s} ", (long)hashcode(node->elem), node->elem, node->order_next->elem);
      }
      else{
         printf("{%ld %s >>NULL} ", (long)hashcode(node->elem), node->elem);
      }
      node = node->table_next;
    }
    printf("\n");
  }
}

/*
Outputs all elements of the hash set according to the order they were added
*/
void hashset_write_elems_ordered(hashset_t *hs, FILE *out){
  hashnode_t* t = hs->order_first;
  //Prints out the first node elem to the file if it is not null
  if(hs->order_first != NULL){
    fprintf(out, "  1 %s\n", t->elem);
  }
  //Loops through the rest of ordered list to print out the rest of nodes to the file
  for(int i = 2; i < hs->elem_count + 1; i++){
    t = t->order_next;
    fprintf(out,"  %d %s\n", i ,t->elem);
  }
}

/*
Writes the given hash set to the given `filename` so that it can be loaded later.  
*/
void hashset_save(hashset_t *hs, char *filename){
  FILE *file = fopen(filename, "w");
  //Checks if the file does not exist and prints the error if so
  if(file == NULL){
    printf("ERROR: could not open file '%s'\n", filename);
  }
  else{
    //prints the table size and elem count to the new file
    fprintf(file, "%d %d\n", hs->table_size, hs->elem_count);
    //prints the ordered node through the hashset_write_elems_ordered function
    hashset_write_elems_ordered(hs, file);
    fclose(file);
  }
}

/*
Loads a hash set file created with hashset_save().
*/
int hashset_load(hashset_t *hs, char *filename){
  FILE *f = fopen(filename, "r");
  //Checks if the file does not exist and prints the error if so and returns 0
  if(f == NULL){
    printf("ERROR: could not open file '%s'\n", filename);
    return 0;
  }
  //Frees the hashset struct from previous information
  hashset_free_fields(hs);
  int temp1;
  int temp2;
  //Scans in the data required to add nodes into the hashset
  fscanf(f, "%d %d\n", &temp1,&temp2);
  hashset_init(hs, temp1);
  char temp[128];
  //Loop that iterates through the file and adds the nodes into the hashset
  for(int i = 0; i < temp2; i++){
    fscanf(f," %*d %s\n", temp);
    hashset_add(hs,temp);
  }
  fclose(f);
  return 1;
}

/*
If 'num' is a prime number, returns 'num'. Otherwise, returns the first prime that is larger than 'num'
*/
int next_prime(int num){
  int isPrime = 0;
  //While loop continues until a prime number returns this boolean-esque integer to 0 from 1
  while(isPrime == 0){
    int isTrue = 0;
    //Checks if the current num is divisible by any number
    for(int i = 2; i <= (num/2); i++){
      if(num % i == 0){
        isTrue = 1;
      }
    }
    //Changes number depending if its even or odd if it was divisible by a number in the previous statement else makes while loop flase
    if(isTrue == 1){
      if(num % 2 == 0){
        num += 1;
      }
      else{
        num += 2;
      }
    }
    else{
      isPrime = 1;
    }
  }
  return num;
}

/*
Allocates a new, larger area of memory for the `table` field and re-adds all current elems to it.
*/
void hashset_expand(hashset_t *hs){
  //Creates a local hashset struct and initializes it
  hashset_t new;
  hashset_init(&new, next_prime(2 * hs->table_size + 1));
  hashnode_t *node1 = hs->order_first;
  hashnode_t *node2;
  //Using tow nodes and iterating through them, adds the old nodes to the new hashet
  while(node1 != NULL){
     node2 = node1->order_next;
     hashset_add(&new, node1->elem);
     node1 = node2;
  }
  //Frees the old hashset field
  hashset_free_fields(hs);
  *hs = new;
}

