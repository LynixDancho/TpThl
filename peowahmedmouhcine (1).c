#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SYMBOLS 256
#define MAX_STATES 256
#define MAX_FILENAME 256

typedef struct Transition
{
    char symbol;
    char destination[MAX_STATES];
    struct Transition *suiv;
} Transition;

typedef struct Etat
{
    bool isFinal;
    char nom[256];
    Transition *head;
} Etat;

typedef struct Automaton
{
    int num_symbols;
    int num_states;
    int num_final_states;
    char symbols[MAX_SYMBOLS];
    char etatinitial[MAX_STATES];
    Etat etats[MAX_STATES];
} Automaton;

bool stringEquals(char *str1, int size1, char *str2, int size2)
{
    return strcmp(str1, str2) == 0;
}

Etat *getState(char name[256], Automaton *automaton)
{
    for (int i = 0; i < automaton->num_states; i++)
    {
        if (stringEquals(automaton->etats[i].nom, 256, name, 256))
        {
            return &automaton->etats[i];
        }
    }
    return NULL;
}

void copyString(char *src, char *destination, int size)
{
    for (int i = 0; i < size; i++)
    {
        destination[i] = src[i];
    }
}

Transition *Create(char symbol, char destination[256])
{
 
    Transition *transition = (Transition *)malloc(sizeof(Transition));

 
    if (transition == NULL)
    {
        printf("BIG ERROR\n");
        return NULL;
    }

 
    transition->symbol = symbol;
    copyString(destination, transition->destination, 256);
    transition->suiv = NULL;

 
    return transition;
}

void addTransition(Transition **head, Transition *toAdd)
{
    Transition *p = *head;
    if (p == NULL)
    {
        *head = toAdd;
        return;
    }

    while (p->suiv != NULL)
    {
        p = p->suiv;
    }
    p->suiv = toAdd;
}
bool isDeterministic(Automaton *automaton)
{
    for (int i = 0; i < automaton->num_states; i++)
    {
        Etat *state = &automaton->etats[i];

        bool seenSymbols[MAX_SYMBOLS] = {false};

        Transition *transition = state->head;

        while (transition != NULL)
        {
            char symbol = transition->symbol;        
            if (seenSymbols[symbol])
            {
                return false;
            }
            seenSymbols[symbol] = true;
            transition = transition->suiv;
        }
    }

    return true;
}


void Affichage(Automaton *automaton)
{
    printf("Nombre de symboles: %d\n", automaton->num_symbols);
    printf("Nombre d'etats: %d\n", automaton->num_states);
    printf("Nombre d'etats finaux: %d\n",   automaton->num_final_states);

    printf("Symboles:");
    for (int i = 0; i < automaton->num_symbols; i++)
    {
        printf(" %c", automaton->symbols[i]);
    }
    printf("\n");

    printf("Etats initial: %s\n", automaton->etatinitial);

    printf("Etats:");
    for (int i = 0; i < automaton->num_states; i++)
    {
        printf(" %s", automaton->etats[i].nom);
    }
    printf("\n");

    printf("Etats Finaux:");
     for (int i = 0; i < automaton->num_states; i++)
    {

        if (automaton->etats[i].isFinal == true)
        {
            printf(" %s", automaton->etats[i].nom);
        }
    }
    printf("\n");

    printf("Transitions:\n");
    for (int i = 0; i < automaton->num_states; i++)
    {
        Etat state = automaton->etats[i];
        Transition *transition = state.head;
        while (transition != NULL)
        {
            printf("delta (%s, %c) %s\n", state.nom, transition->symbol, transition->destination);
            transition = transition->suiv;
        }
    }
    bool isDet = isDeterministic(automaton);
if (isDet)
{
    printf("L'automate est deterministe.\n");
}
else
{
    printf("L'automate n'est pas deterministe.\n");
}

}

void afficherletransition(Etat* etat ,char symbol, Etat* destination){
    printf("/  %s -> %c ->%s", etat->nom,symbol, destination->nom );





}

Etat* Searching(char word , Etat* etat , Automaton* automate){
Transition *transition = etat->head;
while (transition != NULL)
{
    if(transition->symbol== word){
        return getState(transition->destination, automate);
}

transition = transition->suiv;
}
return NULL;





}

bool Accepted( Automaton* automate , char* word){

Etat* etat = getState(automate->etatinitial, automate);   
int wordlength = strlen(word);

for (int i = 0; i < wordlength; i++)
{
Etat* nextetat = Searching(word[i], etat, automate);
if (nextetat == NULL)
{
    return false;
}
afficherletransition(etat, word[i], nextetat);
        etat = nextetat;
}

return etat->isFinal;


}




Automaton *getAutomaton(char *fichier)
{
    Automaton *automaton = (Automaton *)malloc(sizeof(Automaton));
    if (fichier == NULL)
    {
        printf("Error: could not open file %s\n", fichier);
        return NULL;
    }

    Etat *state = (Etat *)malloc(sizeof(Etat));

    FILE *file = fopen(fichier, "r");

    int nombreSymboles, nombreEtats, nombreEtatsFinaux;
    int nombreetatinitial;

    fscanf(file, "Nombre de symboles: %d\n", &nombreSymboles);
    fscanf(file, "Nombre d'etats: %d\n", &nombreEtats);

    fscanf(file, "Nombre d'etats initial: %d\n", &nombreetatinitial);
    fscanf(file, "Nombre d'etats finaux: %d\n", &nombreEtatsFinaux);
    automaton->num_symbols = nombreSymboles;
    automaton->num_states = nombreEtats;
    automaton->num_final_states = nombreEtatsFinaux;

    fscanf(file, "Symboles:");
    for (int i = 0; i < nombreSymboles; i++)
    {
        char symbole;
        fscanf(file, " %c", &symbole);
        automaton->symbols[i] = symbole;
    }
    fscanf(file, "\n");

    char etatinitial[256];
    fscanf(file, "Etats initial:%s", etatinitial);
    copyString(etatinitial, automaton->etatinitial, 256);

    fscanf(file, "\n");

    fscanf(file, "Etats: ");
    for (int i = 0; i < nombreEtats; i++)
    {
        char etat[256];
        fscanf(file, " %s", etat);
        copyString(etat, automaton->etats[i].nom, 256);
    }
    fscanf(file, "\n");

    fscanf(file, "Etats Finaux: ");
    for (int i = 0; i < nombreEtatsFinaux ; i++)
    {
        char etat_fineaux[256];
        fscanf(file, " %s", etat_fineaux);
        Etat *state = getState(etat_fineaux, automaton);
        state->isFinal = true;
    }

    char etat_initial[256], etat_destination[256];
    char symbol;

    fscanf(file, "\n");
    while (EOF != fscanf(file, "delta (%s ,%c) %s\n", etat_initial, &symbol, etat_destination))
    {
        Etat *state = getState(etat_initial, automaton);
        if (state != NULL)
        {
            Transition *toAdd = Create(symbol, etat_destination);

            addTransition(&(state->head), toAdd);
        }
        else
        {
            printf("Source State not found.\n");
        }
    }

    fclose(file);
    return automaton;
}



int main(int argc, char const *argv[])
{
    char file[MAX_FILENAME];
    printf("Entrez le nom du fichier: ");
    scanf("%s", file);
    Automaton *automaton = getAutomaton(file);
    if (automaton == NULL)
    {
        return 1;
    }
    Affichage(automaton);
    
    char word[MAX_FILENAME];
    printf("Entrez le mot a verifer: ");
    scanf("%s", word);
   bool isAccepted = Accepted(automaton, word);
if (isAccepted) {
    printf("\nLe mot est accepte.\n");
} else {
    printf("\nLe mot n est pas accepte.\n");
}

    
    free(automaton);
    return 0;
}