#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


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
Renvoie la liste des char correspondant aux config possibles et prend en arguments la taille du mot
*/
char** get_config_array(int N)
{
    int n = pow(3,N);
    char** config_array = malloc(sizeof(char*)*n);
    char* config;
    for(int i=0;i<n;i++)
    {
        config = malloc(N*sizeof(char));
        int I = i;
        for(int e=0;e<N;e++)
        {
            int r = I%3;
            I=I/3;
            if(r==0)
            {
                config[e] = '0';
            }
            if(r==1)
            {
                config[e] = '1';
            }
            if(r==2)
            {
                config[e] = '2';
            }
        }
        config_array[i] = malloc(sizeof(config));
        config_array[i] = config;
    }
    return(config_array);
}

/*
Afficher un double tableau d'entier
*/
void super_print(char** tab, int N)
{
    for(int i=0;i<pow(3,N);i++)
    {
        printf("%s\n",tab[i]);
    }
}

/*
Renvoie en entier corespondant au nombre d'occurence de letter dans word
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
Revoie true si le mot est compatible avec la config et false sinon
*/
bool is_compatible(char* word_ref, char* word_test, char* config)
{
    bool stop=false;
    char* list_precense = calloc(5,sizeof(char));
    int cursor = 0;

// on test les vert (2)
    for(int i=0; i<5;i++)
    {
        if(config[i]=='2')
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
        if(config[i]=='1')
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
        if (config[i]=='0')
        {
            for (int j=0;j<5;j++)
            {
                if(word_ref[i]==word_test[j])
                {
                    if (strlen(list_precense)>0)
                    {
                        if(occurences(word_ref,word_ref[i])<=occurences(list_precense,word_ref[i]))
                        {
                            return(false);
                        }
                    }
                    else
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
Renvoie le nombre de mot du dico compatible avec word_ref et la config choisie
*/
int get_number_compatible(char* config,char* word_ref,unsigned int size_curr_dico, char** word_array)
{
    int count = 0;
    for(int i=0;i<size_curr_dico;i++)
    {
        char* word_test = word_array[i];
        if(is_compatible(word_ref,word_test,config))
        {
            count++;
        }
    }
    return(count);
}

/*
Renvoie l'entropie de word_ref
*/
double get_entropy(char* word_ref, char** word_array, unsigned int size_curr_dico, char** config_array, int N)
{
    double entropy = 0;
    for(int i=0; i<pow(3,N) ; i++)
    {
        char* config = config_array[i];
        int number_compatible = get_number_compatible(config,word_ref,size_curr_dico,word_array);
        double number_f = number_compatible;
        //printf("%s donne : %f \n",config,number_f);
        double size_f = size_curr_dico;
        double p = number_f/size_f;
        double P = 0;
        if(p!=0)
        {
            //printf("%s donne : %f\n",config,p);
            P = (-p)*(log2(p));
            entropy+=P;
        }
        //printf("%s donne proba : %f et entropie : %f\n",config,p,P);
    }
    return(entropy);
}

char* get_best_word(char** word_array, char** config_array, int N, unsigned int size_curr_dico)
{
    char* best_word = word_array[0];
    double best_entropy = get_entropy(best_word,word_array,size_curr_dico,config_array,N);
    for(int i=0;i<size_curr_dico;i++)
    {
        if(i%10 == 0)
        {
            double ii = i;
            printf("%f pourcent\n",(7.7*ii)/100);
        }
        char* curr_word = word_array[i];
        double curr_entropy = get_entropy(curr_word,word_array,size_curr_dico,config_array,N);
        if(curr_entropy>best_entropy)
        {
            best_entropy=curr_entropy;
            best_word = curr_word;
        }
    }
    return(best_word);
}

void free_tab_char(char** tab,int size_tab)
{
    for(int i=0;i<size_tab;i++)
    {
        free(tab[i]);
    }
    free(tab);
}


int main(int argc, char* argv[])
{
    int N = 5;
    unsigned int size_dico = 7645;
    char** word_array = load_file("dico_5.txt",size_dico);
    char** config_array = get_config_array(N);
    char* best_word = get_best_word(word_array,config_array,N,size_dico);
    printf("%s\n",best_word);
    free_tab_char(word_array,size_dico);
    free_tab_char(config_array,pow(3,N));
    return(0);
}