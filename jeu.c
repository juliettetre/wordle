#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/*
Cette fontion ouvre le fichier et renvois le tableau de mot associé
*/
char **load_file (char *fname, unsigned int size)
{
    char buffer[32];
    char **words_array = malloc(sizeof(char*)*size);
    FILE *f ;

    f = fopen (fname, "rb") ;
    if (f== NULL) return (NULL) ;

    fscanf (f, "%s", buffer) ;
    int indice=0;
    while (! feof (f)) 
    {
        words_array[indice] = malloc(sizeof(buffer));
        strcpy(words_array[indice],buffer);
        fscanf (f, "%s", buffer) ;
        indice++;
    }
    return(words_array);
}





/* 
Cette fonction prend en argument le tableau du dico et sa taille et va renvoyer un mot au hasard du dico
*/
char* word_select(char** dico, int size_dico, char* secret_word)
{
    int id = rand()%size_dico;
    strcpy(secret_word,dico[id]);
    return(secret_word);
}

/*
Cette fonction prend en argument le tablau, sa taille et un mot et renvois si il est dans le dico ou pas
*/
bool recherche_dichoto(char **tab, char *word, unsigned int size)
{
	int a = 0;
	int b = size; 
	int n = size;
	int mid = (a+b)/2;
	while (strcmp(tab[mid],word)!=0 && n!=0)
	{
		if (strcmp(tab[mid],word)<0)
		{
			a = mid;
			mid=(a+b)/2;
		}
		if (strcmp(tab[mid],word)>0)
		{
			b=mid;
			mid=(a+b)/2;
		}
		n=n/2;
		//printf("%d,%d,%d\n",a,b,n);
	}
	if (n==0)
	{
		//printf("PAS dans le dico\n");
		return(false);
	}
	else
	{
		//printf("dans le dico\n");
		return(true);
	}
}

/*
Cette fonction prend en argument le dico et sa taille, demande à l'utilisateur un mot et vérifie si il est valide et le renvois une fois le mot rentré valide
*/
void get_word(char** dico,unsigned int size_dico, char* current_word)
{
    bool stop = false;
    char buffer[16];
    while (! stop)
    {   
        printf("entrez un mot de 5 lettres svp\n");
        scanf(" %s",buffer);
        if (strlen(buffer)!=5)
        {
            printf("le mot ne fait pas 5 lettres\n");
        }
        if (! recherche_dichoto(dico,buffer,size_dico))
        {
            printf("le mot choisi n'est pas dans le dictionnaire\n");
        }
        if (strlen(buffer)==5 && recherche_dichoto(dico,buffer,size_dico))
        {
            stop = true;
        }
    }
    strcpy(current_word,buffer);
}

int occurences (char* word, char letter)
{
    int occurence=0;
    for (int i=0; i<5; i++)
    {
        if (word[i]==letter)
        {
            occurence++;
        }
    }
    return(occurence);
}

void printf_list(char* word)
{
    for(int i=0;i<strlen(word)+1;i++)
    {
        printf("%c",word[i]);
    }
    printf("\n");
}

void  analyse(char* current_word, char* secret_word)
{
    bool presence=false;
    char* list_presents = calloc(5,sizeof(char));
    int cursor=0;
    for (int i=0; i<5; i++)
    {
        if (current_word[i]==secret_word[i])
        {
            printf("%c",current_word[i]);
            list_presents[cursor]=current_word[i];
            cursor++;
        }
        else
        {
            presence=false;
            for (int j=0; j<5; j++)
            {
                if ( j!=i && current_word[i]==secret_word[j])
                {
                    if (occurences(secret_word,current_word[i])>occurences(list_presents,current_word[i]))
                    {
                        presence=true;
                        list_presents[cursor]=current_word[i];
                    }
                }
            }
            if (presence)
            {
                printf(".");
            }
            else
            {
                printf("x");
            }
        }
    }
    printf("\n");
    //printf_list(list_presents);
    free(list_presents);
}



int main(int argc, char* argv[])
{ 
    srand(time(0));
    unsigned int size_dico = 7645 ;
    char** dico = load_file("dico_5.txt",size_dico);
    char word[16];
    char* secret_word = word_select(dico,size_dico,word);
    printf("%s\n",secret_word);
    

    //fin de l'initialisation du jeu, début de la boucle
    int turn = 1;
    bool game = true;
    char current_word[16];
    while (game && turn<=6)
    {
        //on entre un mot et on vérifie qu'il est valide
        get_word(dico,size_dico,current_word);
        if (strcmp(current_word,secret_word)==0)
        {
            game = false;
            printf("Gagnée !! en %d coups\n",turn);
        }
        else
        {
            analyse(current_word,secret_word);
        }
        turn++;
    }
    if (turn>6)
    {
        printf("Vous avez perdu! Le mot était %s\n",secret_word);
    }
    
    return(0);
}