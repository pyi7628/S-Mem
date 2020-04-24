#include <assert.h>             // Needed for assert() macro
#include <stdio.h>              // Needed for printf()
#include <stdlib.h>             // Needed for exit() and ato*()
#include <math.h>               // Needed for pow()
#include <string.h>		// Needed for strcpy()
#include <unistd.h>		// Needed for getopt()

//----- Constants -----------------------------------------------------------
#define  FALSE          0       // Boolean false
#define  TRUE           1       // Boolean true

//----- Function prototypes -------------------------------------------------

char   file_name[256];        // Output file name string
//===== Main program ========================================================
void main(int argc, char *argv[])
{
  FILE   *fp;                   // File pointer to output file
  int    zipf_rv;               // Zipf random variable
  int    i;                     // Loop counter



  if(argc>1) parse_input(argc, argv);
  // Prompt for output filename and then create/open the file
  fp = fopen(file_name, "w");
  if (fp == NULL)
  {
    printf("ERROR in creating output file (%s) \n", file_name);
    exit(1);
  }
 printf("alpha: %lf\n",alpha);
  // Prompt for random number seed and then use it
  rand_val(random_seed);

  // Prompt for alpha value

  // Prompt for N value

  // Prompt for number of values to generate


  int *count=(int*)malloc(sizeof(int)*(n+1));
  for(int j=0;j<=n;j++) 
  {
	  count[j]=0;
  }


  // Generate and output zipf random variables
  for (i=0; i<num_values; i++)
  {
    zipf_rv = zipf(alpha, n);
    fprintf(fp, "%d \n", zipf_rv);
    count[zipf_rv]++;
  }
  /*modifyed by yi 누적분포 구하는 식*/
  double sum=0;
  int flag=0;
  double percentage20 = 0;
  for(int j=1;j<=n;j++)
  {
	  sum += (double)count[j]/(double)num_values;
	  //
	  if((j*100/(int)n) > 20 && flag==0)
	  {
		  percentage20 = sum;
		  flag=1;
	  }
	  printf("%.12lf\n", sum);
  }
  printf("\n--20--: %.12lf\n", percentage20);
  fclose(fp);
}

//===========================================================================
//=  Function to generate Zipf (power law) distributed random variables     =
//=    - Input: alpha and N                                                 =
//=    - Output: Returns with Zipf distributed random variable              =
//===========================================================================
int zipf(double alpha, int n)
{
  static int first = TRUE;      // Static first time flag
  static double c = 0;          // Normalization constant
  double z;                     // Uniform random number (0 < z < 1)
  double sum_prob;              // Sum of probabilities
  double zipf_value;            // Computed exponential value to be returned
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

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
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

void parse_input(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "f:r:a:n:N:")) != -1)
	{
		switch(opt)
		{
			case 'f':
			
				strcpy(file_name, optarg);
				break;
			
			case 'r':
			
				random_seed = atoi(optarg);
				break;

			case 'a': 
				
				alpha = atof(optarg);
				break;
			case 'n':

				n = atof(optarg);
				break;

			case 'N':

				num_values = atoi(optarg);
				break;

			
			default :
				fprintf(stderr, "ERROR: Invalid option.\n");
		}

	}
}
