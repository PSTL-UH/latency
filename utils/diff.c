#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXLINE 120

int main (int argc, char **argv )
{
  typedef struct _table {
    int size;
    double avg, max, min, exectime, minexectime;
    char rest[12];
    struct _table* next;
    struct _table* prev;
  } table;
  table *intab1=NULL, *intab2=NULL, *intab3=NULL;
  table *current, *current2, *current3, *last, *last3;

  FILE *infd1, *infd2, *outfd;

  int done = 0, c, absolute = 0;
  int x1, x2, x;

  double avg1, avg2, min1, min2, max1, max2, exec1, exec2;
  double minexec1, minexec2, mminexec, cminexec;
  double mavg, mmin, mmax, mexec;
  double cavg, cmin, cmax, cexec;

  char inname1[50], inname2[50], outname[50];
  char line[MAXLINE];
  char line1[]="#\n#msg len     average        min           max       avr.-time 1 msg    min. time 1 msg  %deviation\n";
  char line2[]="#   bytes      mbyte/s      mbyte/s       mbyte/s        milli sec          milli sec\n";

  if (argc < 4 )
    {
      printf(" Usage : diff <infilename1> <infilename2> <outfilename> [-a]\n\n");
      printf(" This program calculates the difference between the values given in \n"
	     " <infilename2> and <infilename1>. If a value given in\n"
	     " the second file is not available in the first one\n"
	     " the prograkm interpolates the values using the next lowest\n"
	     " and the next highest value available. The result is written in the new \n"
	     " file <outfilename>.\n\n");
      printf(" Options:\n");
      printf("   -a:  show the difference just as absolute values\n");
      exit ( 1 ) ;
    }

  strcpy ( inname1, argv[1] );
  strcpy ( inname2, argv[2] );
  strcpy ( outname, argv[3] );
  
  infd1 = fopen ( inname1, "r" );
  infd2 = fopen ( inname2, "r" );
  outfd = fopen ( outname, "w" );

  if ( argc == 5 ) {
    c = strcmp ( argv[4], "-a" );
    if ( c == 0 ) absolute = 1;
  }


  /* Read infile 1 and store the values in the according list */
  current = last = NULL;
  while ( fscanf ( infd1, "%[^\n]\n", line ) != EOF ) 
    {
      if ( line[0] == '#' ) {
	/* Skip comment lines */
	continue;
      }
      
      if (current == NULL ) {
	current = (table *) malloc  (sizeof(table));
	current->prev = last;
	if ( last == NULL )  
	  intab1 = current;
	else
	  last->next = current;
      }

      /*read the parameters of the run */
      sscanf ( line, "%d %lf %lf %lf %lf %lf %11s\n", 
	       &current->size, &current->avg, &current->min, &current->max, 
	       &current->exectime, &current->minexectime, &(current->rest[0]) );
      last    = current;
      current = current->next;
    }

#ifdef DEBUG  
  current = intab1;
  /* test by dumping the output */
  while ( current != NULL ) {
    printf ( "%d %lf %lf %lf %lf %lf %s\n", 
	     current->size, current->avg, current->min, current->max,
	     current->exectime, current->minexectime,current->rest );
    current = current->next;
  }
#endif

  /* Read infile 2 and store the values in the according list */
  current = last = NULL;
  while ( fscanf ( infd2, "%[^\n]\n", line ) != EOF ) 
    {
      if ( line[0] == '#' ) 
	continue;
      
      if (current == NULL ) {
	current = (table *) malloc  (sizeof(table));
	current->prev = last;
	if ( last == NULL )  
	  intab2 = current;
	else
	  last->next = current;
      }
      
      /*read the parameters of the run */
      sscanf ( line, "%d %lf %lf %lf %lf %lf %11s\n", 
	       &current->size, &current->avg, &current->min, &current->max, 
	       &current->exectime, &current->minexectime, &(current->rest[0]) );
      last    = current;
      current = current->next;
    }

#ifdef DEBUG  
  current = intab2;
  /* test by dumping the output */
  while ( current != NULL ) {
    printf ( "%d %lf %lf %lf %lf %lf %s\n", 
	     current->size, current->avg, current->min, current->max,
	     current->exectime, current->minexectime, current->rest );
    current = current->next;
  }
#endif


  /* Generate the data for the output file */
  current  = intab1;
  current2 = intab2;
  current3 = last3 = NULL;
  while (!done ) {
    if ( current->size < current2->size ) {
      current = current->next;
    }
    else if ( current->size == current2->size ) {
      if (current3 == NULL ) {
	current3 = (table *) malloc  (sizeof(table));
	current3->prev = last3;
	if ( last3 == NULL )  
	  intab3 = current3;
	else
	  last3->next = current3;
      }
      
      current3->size = current->size;
      current3->avg  = current->avg;
      current3->min  = current->min;
      current3->max  = current->max;
      current3->exectime    = current->exectime;
      current3->minexectime = current->minexectime;

      last3    = current3;
      current3 = current3->next;

      current2 = current2->next;
      current  = current->next;
    }
    else if ( current->size > current2->size ) {
      /* Now we have to interpolate */
      x  = current2->size;
      
      if (current->prev != NULL ) {
	/* last element. We have to use the last two elements to interpolate */
	last = current->prev;
	x1   = last->size;
	x2   = current->size;
	
	avg1 = last->avg;
	avg2 = current->avg;
	min1 = last->min;
	min2 = current->min;
	max1 = last->max;
	max2 = current->max;
	exec1 = last->exectime;
	exec2 = current->exectime;
	minexec1 = last->minexectime;
	minexec2 = current->minexectime;
      }
      else { 
	x1 = current->size;
	x2 = current->next->size;
	
	avg1  = current->avg;
	avg2  = current->next->avg;
	min1  = current->min;
	min2  = current->next->min;
	max1  = current->max;
	max2  = current->next->max;
	exec1 = current->exectime;
	exec2 = current->next->exectime;
	minexec1 = current->minexectime;
	minexec2 = current->next->minexectime;
      }

      mavg  = ( avg2 - avg1)   / ( x2 - x1 );
      mmin  = ( min2 - min1 )  / ( x2 - x1 );
      mmax  = ( max2 - max1 )  / ( x2 - x1 );
      mexec = ( exec2 - exec1) / ( x2 - x1 );
      mminexec = ( minexec2 - minexec1) / ( x2 - x1 );
      
      cavg  = avg1 - mavg * x1;
      cmin  = min1 - mmin * x1;
      cmax  = max1 - mmax * x1;
      cexec = exec1 - mexec * x1;
      cminexec = minexec1 - mminexec * x1;

      if (current3 == NULL ) {
	current3 = (table *) malloc  (sizeof(table));
	current3->prev = last3;
	if ( last3 == NULL )  
	  intab3 = current3;
	else
	  last3->next = current3;
      }

      current3->size     = x;
      current3->avg      = mavg*x+cavg;
      current3->min      = mmin*x+cmin;
      current3->max      = mmax*x+cmax;
      current3->exectime = mexec*x+cexec; 
      current3->minexectime = mminexec*x+cminexec; 

      last3    = current3;
      current3 = current3->next;

      current2 = current2->next;
    }
    
    if ( (current2 == NULL ) || ( current == NULL ) )
      done = 1;
  }

#ifdef DEBUG  
  current = intab3;
  /* test by dumping the output */
  while ( current != NULL ) {
    printf ( "%d %lf %lf %lf %lf %lf %s\n", 
	     current->size, current->avg, current->min, current->max,
	     current->exectime, current->minexectime, current->rest );
    current = current->next;
  }
#endif 

  /* Last step: write the output file : */
  fprintf(outfd, "# Diff-file generated from the files: \n");
  fprintf(outfd, "# %s and %s \n#\n", inname1, inname2 );
  fprintf(outfd, "# Non existent msg. length in the first file have been interpolated\n");
  if ( absolute )
    fprintf(outfd, "# All values are absolute values\n");
  fprintf(outfd, "#\n%s\n", line1 );
  fprintf(outfd, "%s\n", line2 );
  fprintf(outfd, "#-------------------------------------------------------------------------------\n");
  current2 = intab2;
  current3 = intab3;
  while ( current3 != NULL ) {
    if ( absolute )
      fprintf(outfd, "  %.8d     %9.5lf     %9.5lf     %9.5lf     %.5lf    %.5lf\n", 
	      current3->size, 
	      fabs((current3->avg - current2->avg)), 
	      fabs(( current3->min - current2->min)), 
	      fabs(( current3->max - current2->max)), 
	      fabs(( current3->exectime - current2->exectime)),
	      fabs(( current3->minexectime - current2->minexectime))); 
    else
      fprintf(outfd, "  %.8d     %9.5lf     %9.5lf     %9.5lf     %.5lf     %.5lf\n", 
	      current3->size, 
	      (current3->avg - current2->avg), 
	      ( current3->min - current2->min), 
	      ( current3->max - current2->max), 
	      ( current3->exectime - current2->exectime), 
	      ( current3->minexectime - current2->minexectime)); 

      current3 = current3->next;
      current2 = current2->next;
  }

  fclose ( infd1 );
  fclose ( infd2 );
  fclose ( outfd );

  /* Free the memory */
  current = intab1;
  while ( current != NULL ) {
    last = current->next;
    if ( current != NULL ) free ( current );
    current = last;
  }
    
  current = intab2;
  while ( current != NULL ) {
    last = current->next;
    if ( current != NULL ) free ( current );
    current = last;
  }

  current = intab3;
  while ( current != NULL ) {
    last = current->next;
    if ( current != NULL ) free ( current );
    current = last;
  }


  return ( 0 );
}
