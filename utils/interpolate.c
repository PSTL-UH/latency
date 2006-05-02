#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
  table *intab1=NULL, *intab2=NULL;
  table *current, *current2, *last;

  FILE *infd1, *infd2, *outfd;
  char inname1[50], inname2[50], outname[50];
  int done = 0, merge = 0, c;
  char line[MAXLINE];
  int x1, x2, x;
  double avg1, avg2, min1, min2, max1, max2, exec1, exec2;
  double minexec1, minexec2, mminexec, cminexec;
  double mavg, mmin, mmax, mexec;
  double cavg, cmin, cmax, cexec;
  char line1[] ="#   bytes      mbyte/s      mbyte/s       mbyte/s        milli sec          milli sec";

  if (argc < 4)
    {
      printf(" Usage : interpolate <infilename1> <infilename2> <outfilename> [-m]\n\n");
      printf(" This program checks whether the message length used in \n"
	     " <infilename2> are available in <infilename1>. If not, it\n"
	     " interpolates them using the next lowest and the next highest\n"
	     " value. The result is written in the new file <outfilename>.\n\n");
      printf(" Options: \n");
      printf("   -m: merge the new values with the values available in \n"
	     "       <infilename1>.\n");
      exit ( 1 ) ;
    }

  strcpy ( inname1, argv[1] );
  strcpy ( inname2, argv[2] );
  strcpy ( outname, argv[3] );
  
  infd1 = fopen ( inname1, "r" );
  infd2 = fopen ( inname2, "r" );
  outfd = fopen ( outname, "w" );

  if ( argc == 5 ) {
    c = strcmp ( argv[4], "-m" );
    if ( c == 0 ) merge = 1;
  }

  /* Read infile 1 and store the values in the according list */
  current = last = NULL;
  while ( fscanf ( infd1, "%[^\n]\n", line ) != EOF ) 
    {
      if ( line[0] == '#' ) {
	/* Skip comment lines */
	fprintf ( outfd, "%s\n", line );
	c = strncmp ( line, line1, sizeof(line1));
	if ( c == 0 )
	  fprintf (outfd, "# ATTENTION: lines withouth a value for the standard "
		   "deviation are interpolated! \n");

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
	       &current->exectime, &current->minexectime, 
	       &(current->rest[0]) );
      last    = current;
      current = current->next;
    }

#ifdef DEBUG  
  current = intab1;
  /* test by dumping the output */
  while ( current != NULL ) {
    printf ( "%d %lf %lf %lf %lf %lf %s\n", 
	     current->size, current->avg, current->min, current->max,
	     current->exectime, current->minexectime, current->rest );
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
  current = intab1;
  current2 = intab2;
  while (!done ) {
    if ( current->size < current2->size ) {
      if ( merge )
	fprintf(outfd, "  %.8d     %9.5lf     %9.5lf     %9.5lf     %.5lf   %.5lf    %s\n", 
		current->size, current->avg, current->min, current->max, 
		current->exectime, current->minexectime, current->rest ); 
      current = current->next;
    }
    else if ( current->size == current2->size ) {
      fprintf(outfd, "  %.8d     %9.5lf     %9.5lf     %9.5lf     %.5lf   %.5lf   %s\n", 
	      current->size, current->avg, current->min, current->max, 
	      current->exectime, current->minexectime, current->rest ); 
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

      fprintf(outfd, "  %.8d     %9.5lf     %9.5lf     %9.5lf     %.5lf   %.5lf\n", 
	      x, mavg*x+cavg, mmin*x+cmin, mmax*x+cmax, mexec*x+cexec, mminexec*x+cminexec );
      current2 = current2->next;
    }
    
    if ( (current2 == NULL ) && ( current != NULL ) ){
      /* dump the rest of the table */
      while ( current != NULL ) {
	if ( merge )
	  fprintf(outfd, "  %.8d     %9.5lf     %9.5lf     %9.5lf     %.5lf   %.5lf   %s\n", 
		  current->size, current->avg, current->min, current->max,
		  current->exectime, current->minexectime, current->rest );
	current = current->next;
      }
      done = 1;
    }
    else if ( current == NULL )
      done = 1;
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


  return ( 0 );
}
