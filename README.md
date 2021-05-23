# soal-shift-sisop-modul-3-F08-2021

## Anggota Kelompok :
Anggota | NRP
--------|-------------
M. Rayhan Raffi P. | 05111940000110
M. Akmal Joedhiawan | 05111940000125
Nur Moh. Ihsanuddien | 05111940000142

## Penjelasan NO 1
Keverk adalah orang yang cukup ambisius dan terkenal di angkatannya. Sebelum dia menjadi ketua departemen di HMTC, dia pernah mengerjakan suatu proyek dimana keverk tersebut meminta untuk membuat server database buku. Proyek ini diminta agar dapat digunakan oleh pemilik aplikasi dan diharapkan bantuannya dari pengguna aplikasi ini.


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
Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

Penjelasan :
Pertama deklarasikan terlebih dahulu `pthread_ tid[100]`

Lalu membuat fungsi untuk membedakan file dengan directory biasa
````
int cekfile(const char *path) {
	struct stat stat_t;
	stat(path, &stat_t);
	return S_ISREG(stat_t.st_mode);
}
````
Setelah itu terdapat fungsi untuk memindahkan ke dalam directory sesuai dengan ekstensi file masing masing
Untuk opsion `-f` 
````
void* pindah(void *arg) {
	char wdmulai[1024];
	char *path;
	char format[100], dirname[100], a[100], b[100], namafile[100];
	
	path = (char *) arg;
	strcpy(a, arg);
	strcpy(b, arg);
	
	char *split1, *split2, *titik[5], *slash[5];
	int n = 0;

	split1 = strtok(path, ".");
	while(split1 != NULL) {
		titik[n] = split1;
		n++;
		split1 = strtok(NULL, ".");
	}
	if (n == 0){
		strcpy(format, "Hidden");
	}
	
    
	else if(n != 1) {
		int a;
		for(a = 0; a < strlen(titik[n-1]); a++) {
		    format[a] = tolower(titik[n-1][a]);
	    }
	}
	else if (n == 1) {
		strcpy(format, "Unknown");
	}

	n = 0;
	split2 = strtok(b, "/");
	while(split2 != NULL) {
		slash[n] = split2;
		n++;
		split2 = strtok(NULL, "/");
	}
	strcpy(namafile, slash[n-1]);

	char wdtujuan[1024];
	getcwd(wdtujuan, sizeof(wdtujuan));
	strcpy(dirname, wdtujuan);
	strcat(dirname, "/");
	strcat(dirname, format);
	memset(format, 0, 100);
	mkdir(dirname, 0777);

	FILE *psrc, *ptjn;

	psrc = fopen(a, "r");
	strcat(dirname, "/");
	strcat(dirname, namafile);
	ptjn = fopen(dirname, "w");

	if(!psrc) {
		printf("error\n");
	}
	if(!ptjn) {
		printf("error\n");
	}
	int ch;
	while ((ch = fgetc(psrc)) != EOF) {
		fputc(ch, ptjn);
	}

	fclose(psrc);
	fclose(ptjn);
	remove(a);

	return NULL;
}
````

Penjelasan :

- Menggunakan strtok untuk memisahkan string dan membuatnya menjadi token

````
while(split1 != NULL) {
		titik[n] = split1;
		n++;
		split1 = strtok(NULL, ".");
	}
````

- Dikarenakan disoal tidak berlaku case sensitive, kita menggunakan tolower untuk menconvert huruf kapital menjadi huruf kecil. Serta jika file tidak mempuyai ekstensi maka nanti akan membuat folder dengan nama "Unknown" dan apabila hidden maka akan membuat folder "Hidden"

````
if (n == 0){
		strcpy(format, "Hidden");
	}
	
    
	else if(n != 1) {
		int a;
		for(a = 0; a < strlen(titik[n-1]); a++) {
		    format[a] = tolower(titik[n-1][a]);
	    }
	}
	else if (n == 1) {
		strcpy(format, "Unknown");
	}
````

- Kemudian untuk mendapatkan alamat file yang kita inginkan

````
split2 = strtok(b, "/");
	while(split2 != NULL) {
		slash[n] = split2;
		n++;
		split2 = strtok(NULL, "/");
	}
	strcpy(namafile, slash[n-1]);
````

- setelah mendapatkan alamat file, kemudian memindahkan file tersebut kedalam folder

````
char wdtujuan[1024];
	getcwd(wdtujuan, sizeof(wdtujuan));
	strcpy(dirname, wdtujuan);
	strcat(dirname, "/");
	strcat(dirname, format);
	memset(format, 0, 100);
	mkdir(dirname, 0777);

	FILE *psrc, *ptjn;

	psrc = fopen(a, "r");
	strcat(dirname, "/");
	strcat(dirname, namafile);
	ptjn = fopen(dirname, "w");

	if(!psrc) {
		printf("error\n");
	}
	if(!ptjn) {
		printf("error\n");
	}
	int ch;
	while ((ch = fgetc(psrc)) != EOF) {
		fputc(ch, ptjn);
	}

	fclose(psrc);
	fclose(ptjn);
	remove(a);

	return NULL;
}
````

Untuk perintah `-d` dan `-*` karena didalam soal diminta melakukan secara rekursive maka perlu adanya fungsi recursive untuk menjalankannya
````
void listFilesRecursive(char *Pathawal)
{
    char path[1000];
    struct dirent *dp;
    struct stat buffer;
    DIR *dir = opendir(Pathawal);
    int n = 0;

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, Pathawal);
            strcat(path, "/");
            strcat(path, dp->d_name);
            pthread_t thread;
            int err = pthread_create(&thread, NULL, pindahd, (void *)path);
            pthread_join(thread, NULL);
            }

            listFilesRecursive(path);
        }
    closedir(dir);
}
````

Untuk melakukan proses pemindahan file ke dalam folder untuk opsi `-d` dan `-*` hampir sama dengan opsi `-f`, hanya berbeda pada tidak adanya cwd tujuan
````
void *pindahd(void *filename)
{
    char wdmulai[PATH_MAX];
    char dirname[100], hidden[100], hiddenname[100], file[100], existsfile[100];
    int i;
    strcpy(existsfile, filename);
    strcpy(hiddenname, filename);
    char *nama = strrchr(hiddenname, '/');
    strcpy(hidden, nama);

    if (hidden[1] == '.')
    {
        strcpy(dirname, "Hidden");
    }
    else if (strstr(filename, ".") != NULL)
    {
        strcpy(file, filename);
        strtok(file, ".");
        char *token = strtok(NULL, "");
        for (i = 0; token[i]; i++)
        {
            token[i] = tolower(token[i]);
        }
        strcpy(dirname, token);
    }
    else
    {
        strcpy(dirname, "Unknown");
    }

    int exist = cekfile(existsfile);
    if (exist)
        mkdir(dirname, 0755);

    if (getcwd(wdmulai, sizeof(wdmulai)) != NULL)
    {
        char *nama = strrchr(filename, '/');
        char namafile[100];
        strcpy(namafile, wdmulai);
        strcat(namafile, "/");
        strcat(namafile, dirname);
        strcat(namafile, nama);

        rename(filename, namafile);
    }
}
````

Dalam fungsi main
````
int main(int argc, char *argv[]) {
	char wdmulai[1024];
	getcwd(wdmulai, sizeof(wdmulai));

	int x;
	int i;

	if(strcmp(argv[1], "-f") == 0) {
		for(i = 2; i < argc; i++) {
			if(cekfile(argv[i])) {
				pthread_create(&(tid[i-2]), NULL, pindah, (void *)argv[i]);
			}
		}
		for(i = 0; i < argc - 2; i++) {
			x = pthread_join(tid[i], NULL);
			if (x == 0){
				printf("\nFile %d: Berhasil Dikategorikan\n",i+1);}
			else{
				printf("\nFile %d: Sad,Gagal :(\n", i+1);
			}
		}


	}
	else if(strcmp(argv[1], "-d") == 0) {
	listFilesRecursive(argv[2]);
            struct stat buffer;
            int err = stat(argv[2], &buffer);
            if (err == -1)
            {
                printf("Yah, gagal disimpan :(\n");
            }
            else
            {
                printf("Direktori sukses disimpan!\n");
            }
        }
        else if(strcmp(argv[1], "-*") == 0){
        	if (getcwd(wdmulai, sizeof(wdmulai)) != NULL)
            	{
            	    listFilesRecursive(wdmulai);
            	}
	}
	else
	{
		printf("Argumen salah\n");
	}
}
````
Penjelasan :
- Pertama melakukan proses untuk mencari current working directory `getcwd(wdmulai, sizeof(wdmulai))`
- Apabila menerima input `-f` maka akan dicek apabila itu sebuah file atau bukan `cekfile(argv[i])` dan membuat thread menggunakan `pthread_create(&(tid[i-2]), NULL, pindah, (void *)argv[i])` dan dilakukan proses join `pthread_join(tid[i], NULL)` dan melakukan print argumen apabila file telah berhasil di kategorikan
- Apabila menerima input `-d` maka akan dilakukan proses recursive untuk mendapatkan file `listFilesRecursive(argv[2])` setelah itu akan masuk kedalam proses pengekategorian file dan akan melakukan print argumen apabila berhasil dikategorikan maupun gagal dikategorikan
- Apabila menerima input `-*` maka proses yang dilakukan sama dengan input `-d` yaitu melakukan proses recursive `listFilesRecursive(argv[2])` 

### Kendala
Saat pengerjaan kita mengalami beberapa kendala :
- Beberapa kali terkenda eror `segmentation fault`
- Untuk opsi `-d` dan `-*` dilakukan proses recursive, sehingga harus melakukan sedikit perombakan pada fungsi pindahd karena sedikit berbeda dari opsi `-f`

