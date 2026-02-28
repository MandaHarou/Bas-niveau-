#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    int opt;
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    while ((opt = getopt(argc, argv, "amsd")) != -1) {

        switch (opt) {

            case 'a':
                printf("Resultat: %d\n", a + b);
                break;

            case 'm':
                printf("Resultat: %d\n", a * b);
                break;

            case 's':
                printf("Resultat: %d\n", a - b);
                break;

            case 'd':
                printf("Resultat: %d\n", a / b);
                break;

            default:
                printf("Option invalide\n");
        }
    }

    return 0;
}