#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//
void mistake(int mistake){
  if (mistake == 0) {
    fprintf(stderr, "%s\n", "Bad input arguments. EXIT.");
    return;
  }else if (mistake == 1) {
    fprintf(stderr, "%s\n", "Bad function. EXIT.");
    return;
  }else if (mistake == 2) {
    fprintf(stderr, "%s\n", "'X' is not a number or is negative. EXIT.");
    return;
  }else if (mistake == 3) {
    fprintf(stderr, "%s\n", "'Y' is not a number or is negative. EXIT.");
    return;
  }else if (mistake == 4) {
    fprintf(stderr, "%s\n", "'N' is not INTEGER or is negative. EXIT.");
    return;
  }
}
// check if number is FLOAT
int is_number(char const * str){
  char temp[100];
  strcpy(temp, str);

  for (unsigned int i = 0; i < strlen(temp); i++) {
    if (temp[i] < '0' || temp[i] >= '9') {
      if (temp[i] != '.') {
        return 0;
      }
    }
  }

  return 1;
}
// check if number is INTEGER
int is_int(char const * str){
  char temp[100];
  strcpy(temp, str);

  for (unsigned int i = 0; i < strlen(temp); i++) {
    if (temp[i] < '0' || temp[i] >= '9') {
      return 0;
    }
  }

  return 1;
}
// print log
void print_log(double result, double result_taylor, double result_cfrac, double x){
  printf("       log(%lf) = %.12g\n", x, result);
  printf(" cfrac_log(%lf) = %.12g\n", x, result_cfrac);
  printf("taylor_log(%lf) = %.12g\n", x, result_taylor);
}
// print pow
void print_pow(double result, double result_taylor, double result_cfrac, double x, double y){
  printf("         pow(%lf,%lf) = %.12g\n", x, y, result);
  printf("  taylor_pow(%lf,%lf) = %.12g\n", x, y, result_cfrac);
  printf("taylorcf_pow(%lf,%lf) = %.12g\n", x, y, result_taylor);
}
// calculate 'log' with Taylor series
double taylor_log(double x, unsigned int n){
  double result = 0.0;
  if (x > 0 && x < 1) {
    x = 1 - x;
    double up = x;
    for (unsigned int i = 1; i <= n; i++) {
      result -= up / i;
      up *= x;
    }
  }else {
    double up = (x - 1) / x;
    for (unsigned int i = 1; i <= n; i++) {
      result += up / i;
      up *= (x - 1) / x;
    }
  }

  return result;
}
// calculate 'log' with continued fraction
double cfrac_log(double x, unsigned int n){
  double cf = 1.0;
  double a, index;
  x = x - 1;

  for (; n >= 1; n--){
    // get number with remainder and round it up
    index = (double) n / 2;
    index = ceil(index);
    a = x * index * index;
    cf = n + a / cf;
  }

  return x / cf;
}
// run '--log' function
void run_log(double x, unsigned int n){
  double result_taylor, result_cfrac, result;
  result_taylor = taylor_log(x, n);
  result = log(x);
  result_cfrac = cfrac_log(x, n);
  print_log(result, result_taylor, result_cfrac, x);
}
// calculate 'pow' with taylor_log
double taylor_pow(double x, double y, unsigned int n){
  double result = 0.0;
  double lna = taylor_log(x, n);
  double up = y * lna;
  double down = 1;

  for (unsigned int i = 0; i < n; i++) {
    if (i == 0) {
      result += 1;
    }else{
      down *= i;
      result += up / down;
      up *= y * lna;
    }
  }

  return result;
}
// calculate 'pow' with cfrac_log
double taylorcf_pow(double x, double y, unsigned int n){
  double result = 0.0;
  double lna = cfrac_log(x, n);
  double up = y * lna;
  double down = 1;

  for (unsigned int i = 0; i < n; i++) {
    if (i == 0) {
      result += 1;
    }else{
      down *= i;
      result += up / down;
      up *= y * lna;
    }
  }

  return result;
}
// run '--pow' function
void run_pow(double x, double y, unsigned int n){
  // create result variables
  double result_taylor, result_cfrac, result;
  result_taylor = taylor_pow(x, y, n);
  result = pow(x, y);
  result_cfrac = taylorcf_pow(x, y, n);
  print_pow(result, result_taylor, result_cfrac, x, y);
}

int main(int argc, char const *argv[]) {
  // check if we have right amount of input args
  if (argc != 4 && argc != 5) {
    mistake(0);
    return 1;
  }

  // get function and check if it's right
  char const * func = argv[1];
  if (strcmp(func, "--log") != 0 && strcmp(func, "--pow") != 0){
    mistake(1);
    return 1;
  }

  // get X number and check if it's number
  char const * X_str = argv[2];
  if (is_number(X_str) != 1) {
    mistake(2);
    return 1;
  }

  // if we have right amount of args and right function
  if (argc == 4 && strcmp(func, "--log") == 0) {
    // get N number and check if it's INTEGER
    char const * N_str = argv[3];
    if (is_int(N_str) != 1) {
      mistake(4);
      return 1;
    }
    double x = atof(X_str);
    unsigned int n = atoi(N_str);
    run_log(x, n);
  }else if (argc == 5 && strcmp(func, "--pow") == 0){
    char const * N_str = argv[4];
    if (is_int(N_str) != 1) {
      mistake(4);
      return 1;
    }
    // get Y number and check if it's number
    char const * Y_str = argv[3];
    if (is_number(Y_str) != 1) {
      mistake(3);
      return 1;
    }
    double x = atof(X_str);
    double y = atof(Y_str);
    double n = atoi(N_str);
    run_pow(x, y, n);
  }



  return 0;
}
