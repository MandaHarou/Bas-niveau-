#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

void putnbr(int n)
{
    char c;
    if (n >= 10)
        putnbr(n / 10);
    c = (n % 10) + '0';
    write(1, &c, 1);
}

int main(int argc, char *argv[])
{
    int opt;
    int fd;
    char buf[1024];
    ssize_t n;

    while ((opt = getopt(argc, argv, "f:d:r:")) != -1)
    {
        if (optind >= argc)
        {
            write(1, "Missing search word\n", 20);
            return 1;
        }

        char *word = argv[optind];
        size_t wordlen = strlen(word);

        switch (opt)
        {
            case 'f': 
            {
                fd = open(optarg, O_RDWR);
                if (fd == -1)
                {
                    write(1, "Error opening file\n", 19);
                    return 1;
                }

                int count = 0;
                char filebuf[4096]; 

                while ((n = read(fd, filebuf, sizeof(filebuf) - 1)) > 0)
                {
                    filebuf[n] = '\0';
                    char *ptr = filebuf;
                    while ((ptr = strstr(ptr, word)) != NULL)
                    {
                        count++;
                        ptr++;
                    }
                }

                write(1, "File: ", 6);
                write(1, optarg, strlen(optarg));
                write(1, " -> ", 4);
                putnbr(count);
                write(1, " occurrences\n", 13);

                close(fd);
                break;
            }

            case 'd':
            {
                DIR *dir = opendir(optarg);
                if (!dir)
                {
                    write(1, "Error opening directory\n", 24);
                    return 1;
                }

                struct dirent *entry;

                while ((entry = readdir(dir)) != NULL)
                {
                    if (entry->d_type == DT_REG)
                    {
                        char path[1024];
                        strcpy(path, optarg);
                        strcat(path, "/");
                        strcat(path, entry->d_name);

                        fd = open(path, O_RDONLY);
                        if (fd == -1)
                            continue;

                        int count_d = 0;
                        while ((n = read(fd, buf, sizeof(buf) - 1)) > 0)
                        {
                            buf[n] = '\0';
                            char *ptr = buf;
                            while ((ptr = strstr(ptr, word)) != NULL)
                            {
                                count_d++;
                                ptr++;
                            }
                        }

                        close(fd);

                        if (count_d > 0)
                        {
                            write(1, "File: ", 6);
                            write(1, path, strlen(path));
                            write(1, " -> ", 4);
                            putnbr(count_d);
                            write(1, " occurrences\n", 13);
                        }
                    }
                }

                closedir(dir);
                break;
            }
            case 'r':
            {
                fd = open(optarg, O_RDWR);
                if (fd == -1)
                {
                    write(1, "Error opening file or its dost file\n", 19);
                    return 1;
                }

        
                char *filedata = malloc(1000000); 
                if (!filedata)
                {
                    write(1, "Memory error\n", 13);
                    close(fd);
                    return 1;
                }

                n = read(fd, filedata, 1000000 - 1);
                if (n < 0)
                {
                    write(1, "Read error\n", 11);
                    free(filedata);
                    close(fd);
                    return 1;
                }

                filedata[n] = '\0';

        
                char *src = filedata;
                char *dst = filedata;
                while (*src)
                {
                    if (strncmp(src, word, wordlen) == 0)
                        src += wordlen; 
                    else
                        *dst++ = *src++;
                }
                *dst = '\0';


                ftruncate(fd, 0);
                lseek(fd, 0, SEEK_SET);
                write(fd, filedata, strlen(filedata));

                write(1, "Word removed from file: ", 24);
                write(1, optarg, strlen(optarg));
                write(1, "\n", 1);

                free(filedata);
                close(fd);
                break;
            }

            default:
                write(1, "Invalid option\n", 15);
                return 1;
        }
    }

    return 0;
}