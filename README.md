# soal-shift-sisop-modul-3-F08-2021

## Anggota Kelompok :
Anggota | NRP
--------|-------------
M. Rayhan Raffi P. | 05111940000110
M. Akmal Joedhiawan | 05111940000125
Nur Moh. Ihsanuddien | 05111940000142

## Penjelasan NO 1


## Penjelasan NO 2
Crypto (kamu) adalah teman Loba. Suatu pagi, Crypto melihat Loba yang sedang kewalahan mengerjakan tugas dari bosnya. Karena Crypto adalah orang yang sangat menyukai tantangan, dia ingin membantu Loba mengerjakan tugasnya. Detil dari tugas tersebut adalah:
### 2a
**soal :** Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).
```c
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
```
Penjelasan :
- Pertama-tama dibuat shared memory matriks menggunakan `segid=shmget(key,sizeof(int[4][6]),IPC_CREAT|0666);`, lalu dibuat `matrix = (int *)shmat(segid,NULL,0);`.
- Lalu diinputkan kedua matriksnya. Matriks pertama berukuran 4x3, sedangkan matriks kedua berukuran 3x6.
- Lalu loop berikutnya berfungsi untuk melakukan perkalian matriks dan disimpan pada shared memory `matrix[i*columns + j]`.
- Lalu untuk loop terakhir di outputkan hasilnya.

### 2b
**soal :** Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (Catatan!: gunakan shared memory). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya matri(dari paling besar ke paling kecil) (Catatan!: gunakan thread untuk perhitungan di setiap cel). 
Ketentuan :
If a >= b  -> a!/(a-b)!
If b > a -> a!
If 0 -> 0

```c
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
```
Penjelasan :
- Pertama diinputkan matriks baru berukuran 4x6. Lalu dengan segid dan shared memory yang sama dibandingkan kedua array pada fungsi `printperm` menggunakan thread.
- Apabila terjadi case matriks a >= b, maka dilakukan fungi `hasil[tes->row][tes->column] = permutasi(tes->kiri,tes->kanan)`.
- Apabila terjadi case matriks a < b, maka dilakukan fungi `hasil[tes->row][tes->column] = permutasi(tes->kiri,tes->kiri)`.
- Apabila a atau b = 0, maka `hasil[tes->row][tes->column] = 0`.
- Lalu yang terakhir dilakukan pthread_join dan diprintkan hasilnya.

### 2c
**soal :** Karena takut lag dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5” (Catatan!: Harus menggunakan IPC Pipes)

```c
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int pid;
int pipe1[2];
int pipe2[2];

void exec1() {
  // input from stdin (already done)
  // output to pipe1
  dup2(pipe1[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  // exec
  char *argv[]={"ps","aux",NULL};
  execv("/bin/ps",argv);
  // exec didn't work, exit
  perror("bad exec ps");
  _exit(1);
}

void exec2() {
  // input from pipe1
  dup2(pipe1[0], 0);
  // output to pipe2
  dup2(pipe2[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  char *argv[]={"sort","-nrk","3,3",NULL};
  execv("/bin/sort",argv);
  // exec didn't work, exit
  perror("bad exec grep root");
  _exit(1);
}

void exec3() {
  // input from pipe2
  dup2(pipe2[0], 0);
  // output to stdout (already done)
  // close fds
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  char *argv[]={"head","-5",NULL};
  execv("/bin/head",argv);
  // exec didn't work, exit
  perror("bad exec grep sbin");
  _exit(1);
}

int main(int argc,char const *argv[]) {

  // create pipe1
  if (pipe(pipe1) == -1) {
    perror("bad pipe1");
    exit(1);
  }

  // fork (ps aux)
  if ((pid = fork()) == -1) {
    perror("bad fork1");
    exit(1);
  } else if (pid == 0) {
    // stdin --> ps --> pipe1
    exec1();
  }
  // parent

  // create pipe2
  if (pipe(pipe2) == -1) {
    perror("bad pipe2");
    exit(1);
  }

  // fork (grep root)
  if ((pid = fork()) == -1) {
    perror("bad fork2");
    exit(1);
  } else if (pid == 0) {
    // pipe1 --> grep --> pipe2
    exec2();
  }
  // parent

  // close unused fds
  close(pipe1[0]);
  close(pipe1[1]);

  exec3();
  // parent

}
```
Penjelasan :
- Pertama dibuat pipe.
- Kemudian memanggil fungsi yang menjalankan command pertama yaitu `ps aux` untuk meng-list semua proses dan penggunaan status dan sumber daya.
- Lalu command berikutnya yaitu `sort -nrk 3,3` untuk melakukan sort.
- Dan yang terakhir dijalankan command `head -5` untuk menampilkan 5 data teratas yang memakan resource.

### Kendala
Saat pengerjaan kami mengalami beberapa kendala seperti :
1. Terjadi error saat input matriks nomer 2b berupa `segmentation fault`.
2. Fungsi yang tidak selesai pada 2c diakibatkan terlalu banyak fork.


## Penjelasan NO 3

