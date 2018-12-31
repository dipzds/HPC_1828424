#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/*****************************************************************************
  Demonstrates how to crack an encrypted password using a simple "brute force" 
  algorithm. Works on passwords that consist only of 2 uppercase letters and a 
  2 digit integer. 
  Your personalised data set is included in the code. 

  Compile with:
    cc -o pass_crack_orig pass_crack_orig.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./pass_crack_orig > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$36REBSFzq1/dui1N6SgVpJNWbH7.H09nEmDF84hZhfeauI13ShDyaMT/d1tPMVh35NJKKkQrT6OIYDS9VLrIY0",

"$6$KB$y.PArc/oPKxP9VZrVz/PKh1YnIO5Y17fKuUf2fzN/CY9nsu8pM1A1sL1DFqP6opCSrghErWUmBWXeYxzV/yO51",

"$6$KB$IAthvWMIOZKLbD3eyNtKlrD8sY2jOZp5a/iFkhlbh9cWwsLUS735UeNVnRgoAwxrpQEHHMrZIkWAQN.Szo16m0",

"$6$KB$vbtq4gVOUgHj2Z.4IYdvE.Fe.6YaL7/znyuHLWtlZ40ks4xXdesINHl0Pe.uzlsBGs3it9aFo6Ha7eg0Ft4ku."
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the 
 start of the line. Note that one of the most time consuming operations
that 
 it performs is the output of intermediate results, so performance
experiments 
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void crack(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space 
  
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}


int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char *argv[]){
  
  struct timespec start, finish;
  long long int difference;  
  int account = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);

  int i;

  for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }

  time_difference(&start, &finish, &difference);
  printf("accumulated %dp\n", account);
  printf("run lasted for %9.5lfs\n", difference/1000000000.0);

  return 0;
}
