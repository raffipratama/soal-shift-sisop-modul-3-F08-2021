#include<stdio.h>
#include<sys/shm.h>
#include<sys/stat.h>

int *matrix;

int main()
{
key_t key = 1234;
  int rows=4;
  int columns=6;
  int segid,y,i,j,k;
    int array1[4][3];
    int array2[3][6];
segid=shmget(key,sizeof(int[4][6]),IPC_CREAT|0666);
matrix = (int *)shmat(segid,NULL,0);
for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 3; j++)
    {
        scanf("%d",&array1[i][j]);
    }
  }
for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 6; j++)
    {
        scanf("%d",&array2[i][j]);
    }
  }

for(i=0;i<4;i++)    
{    
    for(j=0;j<6;j++)    
    {    
    matrix[i*columns + j]=0;    
        for(k=0;k<3;k++)    
        {
          
        matrix[i*columns + j]+=array1[i][k]*array2[k][j];    
        }    
    }    
}
printf("\n");
for(i=0;i<4;i++)    
{    
    for(j=0;j<6;j++)    
    {    
    printf("%d\t", matrix[i*columns + j]);    
    }    
printf("\n");    
}     
  return 0;
}
