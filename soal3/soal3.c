#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>

pthread_t tid[100];

int cekfile(const char *path) {
	struct stat stat_t;
	stat(path, &stat_t);
	return S_ISREG(stat_t.st_mode);
}



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
