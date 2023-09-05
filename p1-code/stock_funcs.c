// stock_funcs.c: support functions for the stock_main program.

#include "stock.h"

/*
Allocate a new stock struct and initialize its fields.
*/
stock_t *stock_new(){
  //Allocates space in heap memory for struct through malloc function
  stock_t *stockStruct = malloc(sizeof(stock_t));
  //Initialize fields of the structs
  stockStruct->data_file = NULL;
  stockStruct->count = -1;
  stockStruct->prices = NULL;
  stockStruct->min_index = -1;
  stockStruct->max_index = -1;
  stockStruct->best_buy = -1;
  stockStruct->best_sell = -1;
  //Returns the struct created through the parameter given
  return stockStruct;
}

/*
Free a stock.
*/
void stock_free(stock_t *stock){
  //Checks the data_file field of the parameter
  if(stock->data_file != NULL){
    free(stock->data_file);
  }
  //Checks the prices field of the parameter
  if(stock->prices != NULL){
    free(stock->prices);
  }
  //Frees a stock
  free(stock);
  return;
}
/*
Prints data about a stock that is passed in via a pointer.
Employs the brute-force method for printing stock->prices with 5 different possible cases.
*/
void stock_print(stock_t *stock){
  //Checks to ensure that the data_file field of the struct is initialized and print it
  //Else prints out the daat_file field as null
  if(stock->data_file != NULL){
    printf("data_file: %s\n", stock->data_file);
  }
  else{
    printf("data_file: NULL \n");
  }
  printf("count: %d\n", stock->count);
  //If the prices array is not null, print out the prices field based on the number of elements within the array
  //Else, print out that the array is null
  if(stock->prices != NULL){
    //Uses brute force method to print array differently based on the amount of elements in the array with 5 test cases
    if(stock->count == 0){
      printf("prices: []\n");
    } else if(stock->count == 1){
      printf("prices: [%.2f]\n", stock->prices[0]);
    } else if(stock->count == 2){
      printf("prices: [%.2f, %.2f] \n", stock->prices[0], stock->prices[1]);
    } else if(stock->count == 3){
      printf("prices: [%.2f, %.2f, %.2f] \n", stock->prices[0], stock->prices[1], stock->prices[2]);
    } else if(stock->count >= 4){
      printf("prices: [%.2f, %.2f, %.2f, ...] \n", stock->prices[0], stock->prices[1], stock->prices[2]);
    }
  }
  else{
    printf("prices: NULL \n");
  }
  //Print out the rest of the data fields of the parameter struct
  printf("min_index: %d\n", stock->min_index);
  printf("max_index: %d\n", stock->max_index);
  printf("best_buy: %d\n", stock->best_buy);
  printf("best_sell: %d\n", stock->best_sell);
  //If the best_buy and best_sell data fields have been not been determined, then return a profit of 0
  //Else, print the profit generated based on the difference between the two fields
  if(stock->best_buy == -1  && stock->best_sell == -1){
    printf("profit: 0.00\n");
  }
  else{
    double a = stock->prices[stock->best_sell] - stock->prices[stock->best_buy];
    printf("profit: %.2f\n", a);
  }
  return;
}

/*
 Sets the index of 'min_index' and 'max_index' fields of the stock to be the positions in 'prices' of the minimum and maximum values present in it.
*/
void stock_set_minmax(stock_t *stock){
  // Determines if there is a prices array to iterate over. 
  // If so, change the minimum and maximum to the index of the first element of the array
  if(stock->count != 0){
    stock->min_index = 0;
    stock->max_index = 0;
  }
  //Iterates through the prices array to determine the minimum and maximum value index by finding the highest and lowest element index value
  for(int i = 0; i < stock->count; i++){
    //Test for minimumum value by looking for values lower than that of the current index
    if(stock->prices[i] < stock->prices[stock->min_index]){
      stock->min_index = i;
    }
    //Test for maximmum value by looking for values higher than that of the current index
    if(stock->prices[i] > stock->prices[stock->max_index]){
      stock->max_index = i;
    }
  }
  return;
}
  
/*
Sets the 'best_buy' and 'best_sell' fields of 'stock'. This corresponds to the pair which produces the best profit.
*/
int stock_set_best(stock_t *stock){
  double maxProfit = 0;
  //Runs a double for loop that where each buy value is associated with every sell value afterwards to find the 
  //maximum amount of profit on a single stock
  for(int i = 0; i < stock->count; i++){
    for(int j = i + 1; j < stock->count; j++){
      //Checks to see whether the combination of buy and sell is greater than what is currently the maximum profit
      //Maximum profit starts off at 0
      if(-stock->prices[i] + stock->prices[j] > maxProfit){
        stock->best_buy = i;
        stock->best_sell = j;
        maxProfit = -stock->prices[i] + stock->prices[j];
      }
    }
  }
  //Tests for the case where no combination of buy and sell leads to a profit and returns -1 for both fields
  if(maxProfit == 0){
    stock->best_buy = -1;
    stock->best_sell = -1;
    return -1;
  }
  return 0;
}

/*
 Opens file named 'filename' and counts how many times the '\n' newline character appears in it which corresponds to how many lines of text are in it.
*/
int count_lines(char *filename){
  FILE *file = fopen(filename, "r");
  //Checks if the file does not exist and returns -1 and prints the error if true
  if(file == NULL){
    printf("Could not open file '%s'\n", filename);
    return -1;
  }
  else{
    //Parses through the file using the fgetc() to determine the number of lines by counting how mnay time the \n function appears
    int numOfLines = 0;
    char c = fgetc(file);
    //Checks to ensure that the end of the file has not been hit and to stop reading when it has
    while(c != EOF){
      c = fgetc(file);
      if(c == '\n'){
        numOfLines++;
      }
    }
    //Closes the file that was previously opened
    fclose(file);
    return numOfLines;
  }
}

/*
Loads a stock from file 'filename' into 'stock' filling its 'prices' and 'count' fields in.
*/
int stock_load(stock_t *stock, char *filename){
  FILE *fin = fopen(filename, "r");
  double tmp;
  //Checks if the file does not exist and returns -1 and prints the error if true
  if(fin == NULL){
    printf("Could not open file '%s'\n", filename);
    printf("Unable to open stock file '%s', bailing out\n", filename);
    return -1;

  }
  else{
    // Reads the number of lines in file and creates memory space for the array in the heap
    stock->count = count_lines(filename);
    stock->prices = malloc(sizeof(double) * stock->count);
    //Goes through the file and scans the doubles into the price array
    for(int i = 0; i < stock->count; i++) {
      int ret = fscanf(fin, "%*s %lf", &tmp);
      //Checks if reached the end of the file or adds to the array
      if(ret == EOF){
        break;
      }
      else{
        stock->prices[i] = tmp;
      }
    }
    //Changes data_file field to the one mentioned as a parameter
    stock->data_file = strdup(filename);
    fclose(fin);
    return 0;
  }
}

/*
Plots a graphical representation of stock information.
*/
void stock_plot(stock_t *stock, int max_width){
  //Computes and prints the max width, range, and plot_step through stock fields
  printf("max_width: %2i \n", max_width);
  stock_set_minmax(stock);
  int z = stock->max_index;
  int f = stock->min_index;
  double range = stock->prices[z] - stock->prices[f];
  printf("range:      %.2f\n", range);
  double plot_step = range / max_width;
  printf("plot step: %.2f\n", plot_step);
  //Prints out the initial plot information, including the space characters and the plus sign - DOES NOT go to a new line
  printf("                   +");
  //Prints the amount of '-' needed based on the max_width parameter of the function
  for(int i = 0; i < max_width; i++){
    printf("-");
  }
  printf("\n");
  int b = stock->best_buy;
  int c = stock->best_sell;
  //Prints out the the actual stock graph by iterating through the prices array
  for(int j = 0; j < stock->count; j++){
    //Prints the initial part of the graph which is the formatted index of the element in the array
    printf("%3d: ", j);
    //Determines if this is the best_buy
    if(j == b){
      printf("B ");
    }
    //Determines if this is the best_sell
    else if(j == c){
      printf("S ");
    }
    else{
      printf("  ");
    }
    //Determines if this is the maximum price
    if(j == z){
      printf("MAX%8.2f |", stock->prices[j]);
    }
    //Determines if this is the minimum price
    else if(j == f){
      printf("MIN%8.2f |", stock->prices[j]);
    }
    else{
      printf("   %8.2f |", stock->prices[j]);
    }
    //Determines the amount of hashtags to be printed as the bars in the graph
    int width = (stock->prices[j] - stock->prices[f]) / plot_step;
    //Prints bars of the graph
    for(int b = 0; b < width; b++){
      printf("#");
    }
    //Goes to a new line to repeat the process
    printf("\n");
  }
  
}