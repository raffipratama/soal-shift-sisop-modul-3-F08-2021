#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int *matrix;
int long long hasil[4][6];

typedef struct comp{
    int kiri;
    int kanan;
    int row;
    int column;
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
  comp *tes = (comp *)loc;
  if((tes->kiri >=tes->kanan) && (tes->kiri !=0) && (tes->kanan !=0))
        {
          hasil[tes->row][tes->column]= permutasi(tes->kiri,tes->kanan);
        }
  else if((tes->kanan > tes->kiri)  && (tes->kiri !=0) && (tes->kanan !=0))
        {
          hasil[tes->row][tes->column]= permutasi(tes->kiri,tes->kiri);
        }
  else if((tes->kiri==0) || (tes->kanan ==0))
        {
          hasil[tes->row][tes->column] = 0;
        }
}

int main()
{
  pthread_t thr[24];
  key_t key = 1234;
  int rows=4;
  int columns=6;
  int segid,y,i,j,k;
    int array[4][6];
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
          comp *tes = (comp*)malloc(sizeof(*tes));
          tes->kiri = matrix[i*columns + j];
          tes->kanan = array[i][j]; 
          tes->row=i;
          tes->column = j;
          pthread_create(&thr[i*columns + j], NULL, *printperm, (void *)tes ); 
    }
  }

for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 6; j++)
    {
          pthread_join(thr[i*columns + j], NULL);
          printf("%lld\t",hasil[i][j]);
    }
    printf("\n");
  }
shmdt(matrix)  ;
shmctl(segid,IPC_RMID,NULL);
  return 0;
}
