#include<stdio.h>
#include<stdlib.h>
typedef struct {
   int key; /* Record's key */
   long off; /* Record's offset in file */
  } index_S;
typedef struct {
  int siz; /* Hole's size */
  long off; /* Hole's offset in file */
} avail_S;
void fileRead(FILE *fp, long rec_off, int rec_siz, char *buf){
  fseek( fp, rec_off, SEEK_SET );
  fread( &rec_siz, sizeof( int ), 1, fp );
  buf = malloc( rec_siz );
  fread( buf, 1, rec_siz, fp );
  printf("%s",buf);
}
void main() {
  char *buf; /* Buffer to hold student record */
  FILE *fp; /* Input/output stream */
  long rec_off; /* Record offset */
  int rec_siz; /* Record size, in characters */
  if ( ( fp = fopen( "student.db", "r+b" ) ) == NULL ) {
     fp = fopen( "student.db", "w+b" );
   }
   else {
     rec_off = 0;
     fileRead(fp, rec_off, rec_siz, buf);
   }
}
