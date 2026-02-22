#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]){

    char errorOpen[] = "Error : error opening file, verifie auorisation";
    char errorRead[] = "Eroor: error read file, its dosnt exit ";
    char buf[1024];
    size_t nblus;
    int fileread = open(argv[1], O_RDONLY, 0666);
    int filecreat = open(argv[2], O_WRONLY | O_EXCL | O_CREAT, 0666);

    if(argc < 2){
        write(1,"Argument", 8);
        return 1;
    }
    if(fileread < 0){
       int size = sizeof(errorRead);
       write(1, errorRead, size);
       return 2;
    }

   while((nblus = read(fileread, buf, 100)) > 0){
        write(1, buf, nblus);
        if(filecreat < 0){
            write(1, "hello", 5);
            return 3;
        }
        write(filecreat, buf, nblus);

   }
    close(fileread);
    close(filecreat);
    return 0;
}