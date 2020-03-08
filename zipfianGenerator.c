//----- Include files -------------------------------------------------------
#include <assert.h>             // Needed for assert() macro
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()
#include <math.h>               // Needed for pow()

#include "zipfianGenerator.h"

//----- Constants -----------------------------------------------------------
#define  FALSE          0       // Boolean false
#define  TRUE           1       // Boolean true

//----- Function prototypes -------------------------------------------------
//zipfianGenerator.h includes following function prototypes 

//===== Main program ========================================================

void random_value_generator(double S, double N, int number_of_zipfian, int** zipf_arr)//이름 바꾸기!!
{
  char   temp_string[256];      // Temporary string variable
  double alpha = S;                 // Alpha parameter
  double n = N;                     // N parameter
  int    num_values = number_of_zipfian;            // Number of values
  int    zipf_rv;               // Zipf random variable
  int    i;                     // Loop counter
  *zipf_arr = (int*)malloc(sizeof(int) * (num_values+1));
  rand_val(1);//init random number seed

  for(i = 0;i<num_values; i++)
  {
	zipf_rv = zipf(alpha, n);
	*zipf_arr[i] = zipf_rv;
  }
  //fprintf(fp, "%d \n", zipf_rv);//원래 기존 잡는 메모리에 넣을려 했는데 자료형이 안맞는다잉??!, 느리더라도 그냥 file로 쓰고 읽을까
  
}



int zipf(double alpha, int n)//나중에 차라리 chuck size 고정해두고 너무 크지 않게 생성하는게 더 좋을 것 같다...
{
  static int first = TRUE;      // Static first time flag
  static double c = 0;          // Normalization constant
  double z;                     // Uniform random number (0 < z < 1)
  double sum_prob;              // Sum of probabilities
  int  zipf_value;            // Computed exponential value to be returned
  int    i;                     // Loop counter

  // Compute normalization constant on first call only
  if (first == TRUE)
  {
    for (i=1; i<=n; i++)
      c = c + (1.0 / pow((double) i, alpha));
    c = 1.0 / c;
    first = FALSE;
  }

  // Pull a uniform random number (0 < z < 1)
  do
  {
    z = rand_val(0);
  }
  while ((z == 0) || (z == 1));

  // Map z to the value
  sum_prob = 0;
  for (i=1; i<=n; i++)
  {
    sum_prob = sum_prob + c / pow((double) i, alpha);
    if (sum_prob >= z)
    {
      zipf_value = i;
      break;
    }
  }

  // Assert that zipf_value is between 1 and N
  assert((zipf_value >=1) && (zipf_value <= n));

  return(zipf_value);
}


double rand_val(int seed)
{
  const long  a =      16807;  // Multiplier
  const long  m = 2147483647;  // Modulus
  const long  q =     127773;  // m div a
  const long  r =       2836;  // m mod a
  static long x;               // Random int value
  long        x_div_q;         // x divided by q
  long        x_mod_q;         // x modulo q
  long        x_new;           // New x value

  // Set the seed if argument is non-zero and then return zero
  if (seed > 0)
  {
    x = seed;
    return(0.0);
  }

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random value between 0.0 and 1.0
  return((double) x / m);
}
