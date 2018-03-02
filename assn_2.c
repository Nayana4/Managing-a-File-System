#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct {
     int key; /* Record's key */
     long off; /* Record's offset in file */
} index_S;
typedef struct {
  int siz; /* Hole's size */
  long off; /* Hole's offset in file */
} avail_S;
//reqiured for quick sort utility
int cmpfunc (const void * a, const void * b)
{
   index_S *a1 = (index_S *)a;
   index_S *b1 = (index_S *)b;
   return  (int)(a1->key -b1->key );
 }
//reqiured for quick sort utility
int cmpfuncBest (const void * a, const void * b) {
   avail_S *a1 = (avail_S *)a;
   avail_S *b1 = (avail_S *)b;
   return (int)( a1->siz - b1->siz );
}
//reqiured for quick sort utility
int cmpfuncWorst (const void * a, const void * b) {
   avail_S *a1 = (avail_S *)a;
   avail_S *b1 = (avail_S *)b;
   return (int)( b1->siz - a1->siz );
}
int BSearch(index_S arr[], int low, int high, int s){
    if (low<=high){
        int mid = (high+low)/2;
        if(arr[mid].key == s)
            return 1;
        if(arr[mid].key<s)
            return BSearch(arr, mid + 1, high, s);
        else
            return BSearch(arr, low, mid - 1, s);
    }
    return -1;
}
char *studentFileName;
int main(int argc , char *argv[]) {
   studentFileName = argv[2];
   char *buf; /* Buffer to hold student record */
   FILE *fp, *indexfile, *availfile; /* Input/output stream */
   long rec_off; /* Record offset */
   int rec_siz; /* Record size, in characters */
   avail_S availList[10000];
   index_S indexList[10000];
   int i,j ;
   int avaiLastPtr, indexLastPtr, len, fileIndex, key;
   if ( ( fp = fopen( studentFileName, "r+b" ) ) == NULL ) {
       fp = fopen( studentFileName, "w+b" );
   }
   else{
      //Read availabilyList.bin and store it in the availList[] array
      if ( ( availfile = fopen( "availabilyList.bin", "r+b" ) ) == NULL ) {
          availfile = fopen( "availabilyList.bin", "w+b" );
      }
      fseek(availfile,0,SEEK_END);
      len = ftell(availfile)/sizeof(avail_S);
      fseek(availfile,0,SEEK_SET);
      fread(availList,len,sizeof(avail_S), availfile);
      fclose(availfile);
      avaiLastPtr = len;
      //Read indexList.bin and store it in the indexList[] array
      if ( ( indexfile = fopen( "indexList.bin", "r+b" ) ) == NULL ) {
          indexfile = fopen( "indexList.bin", "w+b" );
      }
      fseek(indexfile,0,SEEK_END);
      len = ftell(indexfile)/sizeof(index_S);
      fseek(indexfile,0,SEEK_SET);
      fread(indexList, len,sizeof(index_S), indexfile);
      fclose(indexfile);
      indexLastPtr = len;
   }
   //Read from console
   char line[200], *token, *rec;
   while(strcmp(line,"end\n")!=0){
       fgets(line,200,stdin);
       token = strtok(line," ");
       if(strcmp(token,"add")==0){
              avail_S temp;
              token = strtok(NULL," ");
              key = atoi(token);
              token = strtok(NULL," ");
              rec = token;
              if((BSearch(indexList, 0, indexLastPtr-1, key))==1){
                printf("Record with SID=%d exists\n", key);
              }
              else{
                rec_siz = strlen(rec)-1;
                rec_off = -1;
                if(strcmp("--first-fit",argv[1])==0){
                  for (i = 0; i<avaiLastPtr;i++){
                    if(availList[i].siz > (rec_siz+sizeof(int))){
                      rec_off = availList[i].off;
                      temp.off = availList[i].off + rec_siz + sizeof(int);
                      temp.siz = availList[i].siz-rec_siz - sizeof(int);
                      for (j=i; j<avaiLastPtr-1;j++){
                        availList[j] = availList[j+1];
                      }
                      availList[avaiLastPtr-1] = temp;
                      break;
                    }
                    else if( (availList[i].siz)==rec_siz+sizeof(int)){
                      rec_off = availList[i].off;
                      for (j=i; j<avaiLastPtr-1;j++){
                        availList[j] = availList[j+1];
                      }
                      avaiLastPtr-=1;
                      break;
                    }
                  }
                }
                else if(strcmp("--best-fit",argv[1])==0){
                  for (i = 0; i<avaiLastPtr;i++){
                    if((rec_siz+sizeof(int)) == availList[i].siz){
                      rec_off = availList[i].off;
                      for (j=i; j<avaiLastPtr;j++){
                        availList[j] = availList[j+1];
                      }
                      avaiLastPtr-=1;
                      break;
                    }
                    else if ((rec_siz+sizeof(int)) < availList[i].siz){
                        rec_off = availList[i].off;
                        temp.off = availList[i].off + rec_siz + sizeof(int);
                        temp.siz = availList[i].siz-rec_siz - sizeof(int);
                        for (j=i; j<avaiLastPtr-1;j++){
                            availList[j] = availList[j+1];
                        }
                      availList[avaiLastPtr-1] = temp;
                      qsort(availList, avaiLastPtr , sizeof(avail_S), cmpfuncBest);
                      break;
                    }
                  }
                }
                else if(strcmp("--worst-fit",argv[1])==0){
                  for (i = 0; i<avaiLastPtr;i++){
                    if((rec_siz+sizeof(int)) == availList[i].siz){
                      rec_off = availList[i].off;
                      for (j=i; j<avaiLastPtr;j++){
                        availList[j] = availList[j+1];
                      }
                      avaiLastPtr-=1;
                      break;
                    }
                    else if ((rec_siz+sizeof(int)) < availList[i].siz){
                        rec_off = availList[i].off;
                        temp.off = availList[i].off + rec_siz + sizeof(int);
                        temp.siz = availList[i].siz-rec_siz - sizeof(int);
                        for (j=i; j<avaiLastPtr-1;j++){
                            availList[j] = availList[j+1];
                        }
                        availList[avaiLastPtr-1] = temp;
                        qsort(availList, avaiLastPtr, sizeof(avail_S), cmpfuncWorst);
                        break;
                    }
                  }
                }
                fp = fopen( studentFileName, "r+b" );
                if(avaiLastPtr == -1 || rec_off == -1){

                  fseek(fp, 0, SEEK_END);
                  rec_off = ftell(fp);
                }
                fseek(fp,rec_off,SEEK_SET);
                fwrite(&rec_siz,1,sizeof(int),fp);
                fwrite(rec,rec_siz,1,fp);
                fclose(fp);
                indexList[indexLastPtr].key = key;
                indexList[indexLastPtr].off = rec_off;
                indexLastPtr+=1;
                qsort(indexList, indexLastPtr, sizeof(index_S), cmpfunc);
              }
            }
            else if (strcmp(token,"find")==0) {
              token = strtok(NULL," ");
              rec_siz = 0;
              key = atoi(token);
              if((BSearch(indexList, 0, indexLastPtr-1, key))==-1){
                printf("No record with SID=%d exists\n", key);
              }
              else{
                for (i = 0; i<indexLastPtr;i++){
                  if(key == indexList[i].key){
                    rec_off = indexList[i].off;
                    break;
                  }
                }
                fp = fopen(studentFileName, "r+b" );
            		fseek(fp,rec_off,SEEK_SET);
            		fread(&rec_siz,sizeof(int),1,fp);
            		char *buf = malloc(rec_siz+1);
            		fread(buf,1,rec_siz,fp);
            		printf("%s\n",buf);
                fclose(fp);
              }
            }
            else if(strcmp(token,"del")==0){
              token = strtok(NULL," ");
              key = atoi(token);
              if(((BSearch(indexList, 0, indexLastPtr-1, key))==-1) ||indexLastPtr==0){
                printf("No record with SID=%d exists\n", key);
              }
              else{
                for (i = 0; i<indexLastPtr;i++){
                  if(key == indexList[i].key){
                    rec_off = indexList[i].off;
                    break;
                  }
                }
                while(i<indexLastPtr-1){
                    indexList[i].key = indexList[i+1].key;
                    indexList[i].off = indexList[i+1].off;
                    i++;
                }
                indexLastPtr = indexLastPtr - 1;
                //find the size of the record
                fp = fopen(studentFileName, "r+b" );
            		fseek(fp,rec_off,SEEK_SET);
                fread(&rec_siz,1,sizeof(int),fp);
                //add hole to the availList
                availList[avaiLastPtr].siz = rec_siz+sizeof(int);
                availList[avaiLastPtr].off = rec_off;
                avaiLastPtr += 1;
                if(strcmp("--best-fit",argv[1])==0){
                  qsort(availList, avaiLastPtr, sizeof(avail_S), cmpfuncBest);
                }
                else if(strcmp("--worst-fit",argv[1])==0){
                  qsort(availList, avaiLastPtr, sizeof(avail_S), cmpfuncWorst);
                }
              }
            }
      }

   //write the availList array into the availabilyList.bin file
   fp = fopen( "availabilyList.bin", "w+b" );
   fwrite(availList,sizeof(avail_S), avaiLastPtr, fp);
   fclose(fp);
   //write the index array into the indexList.bin file
   fp = fopen( "indexList.bin", "w+b" );
   qsort(indexList, indexLastPtr, sizeof(index_S), cmpfunc);
   fwrite(indexList,sizeof(index_S), indexLastPtr, fp);
   fclose(fp);

    int hole_num=0;
    printf("Index:\n");
    for(i=0;i<indexLastPtr;i++){
    	printf( "key=%d: offset=%ld\n", indexList[i].key, indexList[i].off );
    }
    printf("Availability:\n");
   	for(i=0;i<avaiLastPtr;i++){
   		printf( "size=%d: offset=%ld\n", availList[i].siz, availList[i].off );
   		hole_num+=availList[i].siz;
   	}
   	printf( "Number of holes: %d\n", avaiLastPtr);
    printf( "Hole space: %d\n", hole_num );
      return 0;
}
