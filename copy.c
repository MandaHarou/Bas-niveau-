#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]){

    char errorOpen[] = "Error : error opening file, verifie auorisation";
    char errorRead[] = "Eroor: error read file, its dosnt exit ";
    char buf[1024];
    size_t nblus;
    int fileReadD = open(argv[1], O_RDONLY, 0666);
    int fileCreatD = open(argv[2], O_WRONLY | O_EXCL | O_CREAT, 0666);

    if(argc < 2){
        write(1,"Argument", 8);
        return 1;
    }
    if(fileReadD < 0){
       int size = sizeof(errorRead);
       write(1, errorRead, size);
       return 2;
    }

   while((nblus = read(fileReadD, buf, 100)) > 0){
        write(1, buf, nblus);
        if(fileCreatD < 0){
            char errorOpen[] = "erro openign file";
            int size = sizeof(errorOpen);
            write(1, errorOpen, size);
            return 3;
        }
        write(fileCreatD, buf, nblus);

   }
    close(fileReadD);
    close(fileCreatD);
    return 0;
}