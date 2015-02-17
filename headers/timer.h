#ifndef TIMER_FUNC
#define TIMER_FUNC

#include <ctime>
#include <iostream>
#include <sys/time.h>

// A pair of functions which act as a timer for any code placed between them.

// Starts the timer. Needs to be assigned to a variable, e.g.
// double start = timerstart()
double timerstart(void)
{
  struct timeval t1;
  gettimeofday(&t1, NULL);
  double start = t1.tv_sec +
    ( t1.tv_usec / 1000000.0 );
  return start;
}

// Ends the timer. Takes the variable produced by timerstart as input,
// and returns the number of seconds since then.
// If the 'output' variable is set to one, prints the time to stdout.
double timerend(double start, int output = 0)
{
  struct timeval t1;
  gettimeofday(&t1, NULL);
  double end = t1.tv_sec +
    ( t1.tv_usec / 1000000.0 );
  double time = end - start;

  if ( output == 1 )
    {
      using namespace std;
      cout << "Time elapsed: " << time << " seconds" << endl;
    }

  return time;
}

// The previous version of the timer.
// Apparently measures 'CPU time' since the start of the program,
// which is usually a lot less than observed time. Not recommended.

double timer(int output = 0)
{
  double time = clock() / (double)CLOCKS_PER_SEC;

  if ( output == 1 )
    {
      using namespace std;
      cout << "CPU time used: " << time << " seconds" << endl;
    }

  return time;
}


#endif
