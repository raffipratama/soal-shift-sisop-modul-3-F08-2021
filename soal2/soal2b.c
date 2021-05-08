#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int *matrix;

typedef struct comp{
    int kiri;
    int kanan;
}comp;

long long rekursi(int n) {
    if (n>=1)
        return n*rekursi(n-1);
    else
        return 1;
}

long long permutasi(int a, int b)
{
  return rekursi(a)/rekursi(a-b);
}

void *printperm(void *loc)
{
  comp *temp = (comp *)loc;
  printf("%lld\t",permutasi(temp->kiri,temp->kanan));
}

void *printfac(void *loc)
{
  comp *temp = (comp *)loc;
  printf("%lld\t",rekursi(temp->kiri));
}

int main()
{
  pthread_t thr;
  key_t key = 1234;
  int rows=4;
  int columns=6;
  int segid,y,i,j,k;
    int array[4][6];
    long long hasil[4][6];
segid=shmget(key,sizeof(int[4][6]),IPC_CREAT|0666);
//printf("%d",segid);
matrix = (int *)shmat(segid,NULL,0);
int count = 1;
for (i = 0; i < 4; i++)
  {
     
    for (j = 0; j < 6; j++)
    {
        count++;
        scanf("%d",&array[i][j]);
    } 
    
  }
 
for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 6; j++)
    {
        if((matrix[i*columns + j]>=array[i][j]) && (matrix[i*columns + j]!=0) && (array[i][j]!=0))
        {
          comp *tes = (comp*)malloc(sizeof(*tes));
          tes->kiri = matrix[i*columns + j];
          tes->kanan = array[i][j];
          pthread_create(&thr, NULL, *printperm, (void *)tes ); 
          pthread_join(thr,NULL);
        }
        else if((array[i][j]>matrix[i*columns + j]  && (matrix[i*columns + j]!=0) && (array[i][j]!=0)))
        {
          comp *tes = (comp*)malloc(sizeof(*tes));
          tes->kiri = matrix[i*columns + j];
          tes->kanan = array[i][j];
          pthread_create(&thr, NULL, *printfac, (void *)tes ); 
          pthread_join(thr,NULL);
        }
        else if((matrix[i*columns + j]==0) || (array[i][j]==0))
        {
          hasil[i][j] = 0;
          printf("0\t"); 
        }
    }
    printf("\n");
  }

shmdt(matrix)  ;
shmctl(segid,IPC_RMID,NULL);
  return 0;
}
