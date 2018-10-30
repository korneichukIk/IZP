#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 1024
// global variables: data to modify, commands, amount of original data rows, amount of commands
char vstup[MAX_CHAR][MAX_CHAR];
char commands[MAX_CHAR][MAX_CHAR];
int counter_vstup;
int counter_commands;
// convert string to int
int get_number(char * str){
  int result;

  result = 0;

  while ((*str >= '0') && (*str <= '9')) {
    result = (result * 10) + ((*str) - '0');
    str++;
  }

  return result;
}
// printing of mistakes
void mistake(int mistake){
  if (mistake == 1) {
    printf("Bad arguments. EXIT.\n");
  }else if (mistake == 2) {
    printf("FILE doesn't exists. EXIT.\n");
  }else if(mistake == 3){
    printf("Bad arguments in function 'b'. EXIT.\n");
  }else if (mistake == 4) {
    printf("Bad arguments in function 'a'. EXIT.\n");
  }else if (mistake == 5) {
    printf("Bad arguments in function 'g'. EXIT.\n");
  }else if (mistake == 6) {
    printf("Bad arguments in function 'n'. EXIT.\n");
  }else if (mistake == 7) {
    printf("Bad arguments in function 'd'. EXIT.\n");
  }else if (mistake == 8) {
    printf("Bad arguments in function 'i'. EXIT.\n");
  }else if (mistake == 9) {
    printf("Bad arguments in function 's'. EXIT.\n");
  }else if (mistake == 10) {
    printf("File with commands is empty. EXIT.\n");
  }else if (mistake == 11) {
    printf("Bad command. EXIT.\n");
  }else if (mistake == 12) {
    printf("Bad arguments in function 'f'. EXIT.\n");
  }
}
// readfile function. Reads file content and save it to variables.
// return amount of rows in file
int readfile(char * filename){
  FILE *file;
  char * line = NULL;
  int counter = 0;
  size_t len = 0;

  // open file
  file = fopen(filename, "r");
  // if file doesn't exists
  if(file == NULL){
    mistake(2);
    exit(EXIT_FAILURE);
  }

  // reading file line by line
  while (getline(&line, &len, file) != -1) {
    if (strcmp(filename, "vstup") == 0) {
      strcpy(vstup[counter], line);
    }else{
      strcpy(commands[counter], line);
    }
    counter++;
  }

  // check if file is not empty
  if (counter == 0) {
    mistake(10);
    exit(EXIT_FAILURE);
  }

  // close file and return
  fclose(file);
  return counter;
}
// function used for exiting
// prints finall result
void quit(int actual_row_number){
  for (int i = 0; i < actual_row_number; i++) {
    printf("%s", vstup[i]);
  }
  return;
}
// replace 'pattern' to 'replacment' in original data row with index 'actual_row_number'
void replace_str(int actual_row_number, char * pattern, char * replacment){
  char buffer[1024] = { 0 };
  char * insert_point = &buffer[0];
  const char * temp = vstup[actual_row_number];
  size_t pattern_len = strlen(pattern);
  size_t repl_len = strlen(replacment);

  // we are looking for PATTERN in the row
  while (1) {
    const char * p = strstr(temp, pattern);

    // if we don't find, we save same row and exit
    if (p == NULL) {
      strcpy(insert_point, temp);
      break;
    }

    // we copy our row to the temp variable
    memcpy(insert_point, temp, p - temp);
    // we are indexing our temp variable
    insert_point += p - temp;

    // we copy replacment to temp variable from index we put above
    memcpy(insert_point, replacment, repl_len);
    insert_point += repl_len;


    temp = p + pattern_len;
  }

  // we copy our temp variable to our original row
  strcpy(vstup[actual_row_number], buffer);
}
// inser substring 'substring' to data row with id 'actual_row_number'
// position = 0 if we insert it on the beggining of the row
// position = 1 if we insert it on the end of the row
void insert_substring(char substring[], int actual_row_number, int position){
  if (position == 0) {
    // copy substring to temp
    int len = strlen(substring);
    char temp[len];
    strcpy(temp, substring);

    // if last symbol is EOL we remove it
    if (temp[len-1] == '\n') {
      memmove(&temp[len-1], &temp[len], 1);
    }
    // adding original row to temp
    strcat(temp, vstup[actual_row_number]);
    // coping temp to original row
    strcpy(vstup[actual_row_number], temp);
    return;
  }else{
    int len = strlen(substring);
    int len_vstup = strlen(vstup[actual_row_number]);
    char temp[len];
    strcpy(temp, substring);

    // if first last symbol of original row is EOL, we remove it
    if(vstup[actual_row_number][len_vstup - 1] == '\n'){
      memmove(&vstup[actual_row_number][len_vstup - 1], &vstup[actual_row_number][len_vstup], 1);
    }
    // adding temp to original row
    strcat(vstup[actual_row_number], temp);
  }
}
// checking if string if number
int is_digit(char string[]){
  for (int unsigned i = 0; i < strlen(string); i++) {
    if(string[i] > '0' && string[i] <= '9'){
      return 1;
    }else{
      return 0;
    }
  }
  return 0;
}
// implements command 'r'
void command_r(int actual_row_number){
  int len = strlen(vstup[actual_row_number]);

  // remove last symbol in the row if it is EOL
  if (vstup[actual_row_number][len - 1] == '\n') {
    memmove(&vstup[actual_row_number][len-1], &vstup[actual_row_number][len], 1);
  }
}
// implements functions 'a' and 'b'
void command_insert(int actual_row_number, int actual_command_number, char command){
  int unsigned len = strlen(commands[actual_command_number]);
  char substring[len];
  int mistake_id;
  int label;

  if (command == 'a') {
    mistake_id = 4;
    label = 1;
  }else if (command == 'b') {
    mistake_id = 3;
    label = 0;
  }

  // if CONTENT is not empty
  if (len > 2) {
    strncpy(substring, commands[actual_command_number] + 1, len);
    insert_substring(substring, actual_row_number, label);
  }else{
    mistake(mistake_id);
    exit(EXIT_FAILURE);
  }
}
// implements functions 'n'
// jumping to other rows
int command_n(int actual_command_number){
  int unsigned len = strlen(commands[actual_command_number]);
  char substring[len];
  if (len > 2) {
    strncpy(substring, commands[actual_command_number] + 1, strlen(commands[actual_command_number]));
    // if number is digit
    if (is_digit(substring)) {
      // return number
      return get_number(substring);
    }else{
      mistake(6);
      exit(EXIT_FAILURE);
    }
  }else{
    // if no number
    return 1;
  }
}
// implements command 'g'
// jump to another command
// returns index of command
int command_g(int actual_command_number){
  int unsigned len = strlen(commands[actual_command_number]);
  char substring[len];
  // if gX is not empty
  if (len > 2) {
    strncpy(substring, commands[actual_command_number] + 1, len);
    // if X is digit
    if (is_digit(substring)) {
      // if X >= amount of commands and last command is not 'g' or not 'q'
      if (get_number(substring) > counter_commands && commands[counter_commands - 1][0] != 'g') {
        if (commands[counter_commands - 1][0] != 'q') {
          mistake(5);
          exit(EXIT_FAILURE);
        }
      }

      return get_number(substring);
    }else{
      mistake(5);
      exit(EXIT_FAILURE);
    }
  }else{
    mistake(5);
    exit(EXIT_FAILURE);
  }
  return 0;
}
// implements function 'd'
// deletion of actual row, or some rows
void command_d(int actual_row_number, int actual_command_number){
  // amount to delete
  int to_delete;
  int unsigned len = strlen(commands[actual_command_number]);
  char substring[len];
  if (len >= 2) {
    strncpy(substring, commands[actual_command_number] + 1, len);
    // if content is number
    if(is_digit(substring)){
      to_delete = get_number(substring);
      // if content > amount of rows below
      if (to_delete >= counter_vstup - actual_row_number - 1){
        to_delete = counter_vstup - actual_row_number;
      }
      
    }else{
      mistake(7);
      exit(EXIT_FAILURE);
    }
  }else{
    to_delete = 1;
  }

  // shifting of original data in quantity of elements we have to delete
  for (int i = actual_row_number; i < actual_row_number + to_delete; i++) {
    // shifting every single row
    for (int j = actual_row_number; j < counter_vstup; j++) {
      strcpy(vstup[j], vstup[j+1]);
    }
    // decrease original data amount by 1
    counter_vstup--;
  }
}
// implements command 'i'
// append string CONTENT to row above actual
int command_i(int actual_row_number, int actual_command_number){
  int unsigned len = strlen(commands[actual_command_number]);
  char substring[len];
  // if CONTENT is not empty
  if (len >= 2) {
    strncpy(substring, commands[actual_command_number] + 1, len);
  }else{
    mistake(8);
    exit(EXIT_FAILURE);
  }

  // shifting all rows from actual to the last
  for (int i = counter_vstup - 1; i >= actual_row_number; i--) {
    strcpy(vstup[i+1], vstup[i]);
  }

  // copy CONTENT to the empty row we get
  strcpy(vstup[actual_row_number], substring);

  // return actual row id
  return actual_row_number;
}
// implements commands 's' and 'S'
void command_s(int actual_row_number, int actual_command_number, char command){
  char split;
  int unsigned len = strlen(commands[actual_command_number]);
  char substring[len];

  // if PATTERN and REPLACMENT are not empty
  if (len > 2) {
    strncpy(substring, commands[actual_command_number] + 1, len);
  }else{
    mistake(9);
    exit(EXIT_FAILURE);
  }


  // split = first SYMBOL in CONTENT(PATTERN + REPLACMENT)
  split = substring[0];
  // getting splited pattern and replacment
  char * pattern = strtok(substring, (char[2]) {(char) split, '\0'});
  char * replacment = strtok(NULL, (char[2]) {(char) split, '\0'});

  if (replacment == NULL)
  {
    mistake(9);
    exit(EXIT_FAILURE);
  }

  // if last symbol in REPLACMENT is EOL, we remove it
  for (int unsigned i = 0; i < strlen(replacment); i++) {
    if (replacment[i] == '\n') {
      replacment[i] = (char) 0;
      break;
    }
  }

  // if we have command 's'
  if (command == 's') {
    replace_str(actual_row_number, pattern, replacment);
    return;
  }

  // if we have command 'S'
  else if (command == 'S') {
    for (int i = 0; i < counter_vstup; i++) {
      replace_str(i, pattern, replacment);
    }
    return;
  }
}
// implements command 'f'
int command_f(int actual_row_number, int actual_command_number){
  int result;
  int unsigned len = strlen(commands[actual_command_number]);
  char substring[len];


  // if PATTERN is not empty
  if (len > 2) {
    strncpy(substring, commands[actual_command_number] + 1, strlen(commands[actual_command_number]));
  }else{
    mistake(12);
    exit(EXIT_FAILURE);
  }
  //remove EOL from PATTERN in order to find right place
  if (substring[len - 2] == '\n') {
    memmove(&substring[len - 2], &substring[len - 1], 2);
  }

  for (int i = actual_row_number; i < counter_vstup; i++) {
    const char * p = strstr(vstup[i], substring);
    if (p != NULL) {
      result = i;
      break;
    }
  }

  return result;
}
// implements command 'e'
void command_e(int actual_row_number){
  char * replacment = "\n\0";
  strcat(vstup[actual_row_number], replacment);
}

void run(){
  // variable for describing your position in the files
  int actual_command_number = 0;
  int actual_row_number = 0;
  int iter = 0;

  // main endless cycle
  while (1) {
    // if we have command 'q' (quit)
    if (commands[actual_command_number][0] == 'q') {
      quit(actual_row_number);
      return;
    }
    // if we don't have more commands or if we don't have more strings to modify
    else if (actual_row_number >= counter_vstup || actual_command_number >= counter_commands) {
      quit(counter_vstup + 1);
      return;
    }
    // if we have command 'b'
    else if (commands[actual_command_number][0] == 'b') {
      command_insert(actual_row_number, actual_command_number, 'b');
      actual_command_number += 1;
      continue;
    }
    // if we have command 'a'
    else if (commands[actual_command_number][0] == 'a') {
      command_insert(actual_row_number, actual_command_number, 'a');
      actual_command_number += 1;
      continue;
    }
    // if we have command 'd'
    else if (commands[actual_command_number][0] == 'd') {
      command_d(actual_row_number, actual_command_number);
      // decrease amount of original rows
      counter_vstup --;
      actual_command_number += 1;
    }
    // if we have command 'n'
    else if (commands[actual_command_number][0] == 'n') {
      // updating our actual row with variable of 'n' function
      actual_row_number += command_n(actual_command_number);
      actual_command_number += 1;
      iter ++;
      if (iter >= 500) {
        quit(counter_vstup + 1);
        return;
      }
      continue;
    }
    // if we have command 'g'
    else if (commands[actual_command_number][0] == 'g') {
      actual_command_number = command_g(actual_command_number) - 1;
      iter ++;
      // simple protection from looping
      if (iter >= 500) {
        quit(counter_vstup + 1);
        return;
      }
      continue;
    }
    // if we have command 'r'
    else if (commands[actual_command_number][0] == 'r') {
      command_r(actual_row_number);
      actual_command_number += 1;
      continue;
    }
    // if we have command 'i'
    else if (commands[actual_command_number][0] == 'i'){
      actual_row_number = command_i(actual_row_number, actual_command_number) + 1;
      actual_command_number += 1;
      counter_vstup += 1;
    }
    // if we have command 's'
    else if (commands[actual_command_number][0] == 's') {
      command_s(actual_row_number, actual_command_number, 's');
      actual_command_number += 1;
    }
    // if we have command 'S'
    else if (commands[actual_command_number][0] == 'S') {
      command_s(actual_row_number, actual_command_number, 'S');
      actual_command_number += 1;
    }
    // if we have command 'f'
    else if (commands[actual_command_number][0] == 'f') {
      actual_row_number = command_f(actual_row_number, actual_command_number);
      actual_command_number += 1;
    }
    // if we have command 'e'
    else if (commands[actual_command_number][0] == 'e') {
      command_e(actual_row_number);
      actual_command_number += 1;
    }
    // if we don't find any right command
    else{
      mistake(11);
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char const *argv[]) {
  // check if we have right amount of arguments
  if (argc != 2) {
    mistake(1);
    return 1;
  }

  // create names for files
  char commands_filename[strlen(argv[1])];
  char * vstup_filename = "vstup";

  strcpy(commands_filename, argv[1]);

  // read and save all data from files
  counter_vstup = readfile(vstup_filename);
  counter_commands = readfile(commands_filename);

  // begin main program
  run();


  return 0;
}
