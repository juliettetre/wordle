#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    FILE* dico = fopen(argv[1],"rb");
    FILE* dico_bis = fopen(argv[2],"wb");
    char buffer[128];
    while (! feof(dico))
    {
        fscanf(dico,"%s",buffer);
        if (strlen(buffer)==5)
        {
            fprintf(dico_bis,"%s\n",buffer);
        }
    }
    fclose(dico);
    fclose(dico_bis);
    return(0);

}