#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


struct word_list_t 
{
  char *word ;
  struct word_list_t *next ;
};

char **load_file (char *fname, unsigned int *size)
{
  char **words_array ;
  unsigned int count_words = 0 ;
  struct word_list_t *wl = NULL ;
  char buffer[256] ;
  FILE *in_hd ;

  /* First open the file and check success. */
  in_hd = fopen (fname, "rb") ;
  if (in_hd == NULL) return (NULL) ;

  /* Read the first word. */
  fscanf (in_hd, "%s", buffer) ;
  while (! feof (in_hd)) 
  {
    char *word_cpy ;
    struct word_list_t *new_cell = malloc (sizeof (struct word_list_t*)) ;
    if (new_cell == NULL) 
    {
      /* Close the opened file. */
      fclose (in_hd) ;
      /* BAD: We should also free the already allocated string list cells, but
         I'm too lazzy ^^ Don't do this at home... */
      return (NULL) ;
    }
    /* Allocate memory to store the word. */
    word_cpy = malloc ((1 + strlen (buffer)) * sizeof (char)) ;
    if (word_cpy == NULL) 
    {
      /* BAD: Same comment than above. */
      fclose (in_hd) ;
      return (NULL) ;
    }
    /* Copy the buffer into the fresh string. */
    strcpy (word_cpy, buffer) ;
    new_cell->word = word_cpy ;
    /* Now, link the cell. */
    new_cell->next = wl ;
    wl = new_cell ;
    count_words++ ;

    /* Read the next word. */
    fscanf (in_hd, "%s", buffer) ;
  }

  /* Now, close the file since we do not need it anymore. */
  fclose (in_hd) ;

  /* It's now time to transfer the string into an array. Attention, the list
     construction has reversed the order of the words. */
  words_array = malloc (count_words * sizeof (char**)) ;
  if (words_array == NULL) {
    /* BAD: We should free the already allocated memory. */
    return (NULL) ;
  }

  /* Store the size now since we will use 'count_words' to fill the array. */
  *size = count_words ;

  while (count_words != 0) {
    /* Attention: by invariant, wl->next is never NULL while in the loop. This
       is due to the fact that the length of the list is exactly
       'count_words'. */
    struct word_list_t *tmp = wl->next ;
    count_words-- ;
    /* Copy the pointer hence share the string. So, do not free it now ! */
    words_array[count_words] = wl->word ;
    /* Go to the next cell of the list. */
    wl = tmp ;
  }

  return (words_array) ;
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
        char* p = scanf(" %s",buffer);
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
    unsigned int size_dico ;
    char** dico = load_file("dico_5.txt",&size_dico);
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