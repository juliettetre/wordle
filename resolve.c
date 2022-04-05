#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/*
HONTEUX... à modifier .........
*/
int** get_config_array(int N)
{
    int** config_array = malloc(sizeof(int*)*243);
    int c=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            for(int k=0; k<3;k++)
            {
                for(int l=0; l<3;l++)
                {
                    for(int m = 0; m<3 ; m++)
                    {
                        int* L = malloc(sizeof(int)*N);
                        L[0]=i;
                        L[1]=j;
                        L[2]=k;
                        L[3]=l;
                        L[4]=m;
                        //printf("%d %d %d %d %d \n",i,j,k,l,m);
                        config_array[c] = malloc(sizeof(L));
                        config_array[c] = L;
                        c++;
                    }
                }
            }
        }
    }
    //printf("%d \n",c);
    return(config_array);
}


/*
afficher un double tableau d'entier
*/
void super_print(int** tab)
{
    for(int i=0;i<243;i++)
    {
        for(int j=0;j<5;j++)
        {
            printf("%d",tab[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
renvois en entier corespondant au nombre d'occurence de letter dans word
*/
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

/*
Revois true si le mot est compatible avec la config et false sinon
*/
bool is_compatible(char* word_ref, int* config, char* word_test)
{
    bool stop=false;
    char* list_precense = calloc(5,sizeof(char));
    int cursor = 0;

// on test les vert (2)
    for(int i=0; i<5;i++)
    {
        if(config[i]==2)
        {
            if(word_ref[i]==word_test[i])
            {
                list_precense[cursor]=word_ref[i];
                cursor++;
            }
            else
            {   
                return(false);
            }
        }
    }
    //printf("on est au test 1 \n");
    //on test les jaune (1)
    for(int i=0;i<5;i++)
    {
        stop = true;
        if(config[i]==1)
        {
            stop = false;
            for (int j=0;j<5;j++)
            {
                if(! stop)
                {
                    if(word_ref[i]==word_test[j] && i!=j)
                    {
                        //printf("ref : %d list : %d\n",occurences(word_ref,word_ref[i]),occurences(list_precense,word_ref[i]));
                        //printf("c'est i : %d\n",i);
                        //printf("c'est j : %d\n",j);
                        if(occurences(word_ref,word_ref[i])>occurences(list_precense,word_ref[i]))
                        {
                            list_precense[cursor] = word_ref[i];
                            stop = true;
                        }
                        else
                        {
                            //printf("au premier \n");
                            return(false);
                        }
                    }
                }
            }
        }
        if (! stop)
        {
            //printf("on est sortit au stop\n");
            return(stop);
        }
    }
    //printf("on est au test 0 \n");
    
    //on test les gris (0)
    for(int i=0;i<5;i++)
    {
        if (config[i]==0)
        {
            for (int j=0;j<5;j++)
            {
                if(word_ref[i]==word_test[j])
                {
                    if(occurences(word_ref,word_ref[i])<=occurences(list_precense,word_ref[i]))
                    {
                        return(false);
                    }
                }
            }
        }
    }
    //printf("%d \n",cursor);
    return(true);
}

/*
int get_entropy(char* word, char** word_array, unsigned int size, int** config_array)
{


    for(int id_config = 0; id_config < 243 ; id_config++)
    {

    }

}
*/














int main(int argc, char* argv[])
{
    
    
    int** A = get_config_array(5);
    super_print(A);
    bool T = is_compatible("ZZEEE",A[4],"YTTEY");
    printf("%d \n",T);
    return(0);
}