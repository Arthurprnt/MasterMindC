#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* Constantes */

#define NT  12   /* Nb de propositions maximales */
#define LC  4    /* Longueur du code */
const int NC = 8;    /* Pions de valeur 0 à  NC-1 */

/* Type de la reponse */

struct s_rep { // Type pour les réponses
	int bp;
	int bc;
};
typedef struct s_rep reponse;

/* Variables globales */

int Plateau[NT][LC] = {{0}}; /* Plateau des propositions */
reponse Rep[NT] = {{.bp = 0, .bc = 0}}; /* Réponses associées */
bool modeIA = true; // faire jouer l'IA ?

/* Déclaration de fonctions */

void afficheTab(int tab[], int n); // Affiche les valeurs d'un tableau sur une seule ligne
void affichePlateau(int ncoups); // Affiche le plateau de jeu, sachant que ncoups ont été joués
void creeCombinaison(int tab[], int n); // Remplit un tableau avec des valeurs aléatoires entre 0 et NC-1
void choixCombinaison(int tab[], int n); // Remplit un tableau avec des valeurs indiquées par l'utilisateur
void choixCombinaisonIA(int tab[], int n, int nbcoups); // Remplit un tableau avec les valeurs indiquées pat l'IA
reponse calculeReponse(int code[], int prop[], int n); // Renvoie une réponse pour un code caché et une proposition 

/* Programme principal */

int main() {

	int nbTour = 0;
	int codeTrouve = 0;
    
    srand(time(NULL)); // Obligatoire pouur avoir un "vrai" aléatoire
	printf("\n     Jeu du mastermind\n\n===========================\n");
	
    int code[LC] = {0};
    creeCombinaison(code, LC);
    afficheTab(code, LC);
	printf("  CODE DE LA PARTIE\n");

	// déroulement du jeu
	while (nbTour+1 < NT && !codeTrouve) {

		int Cj[LC] = {NC}; // Initialisation de la prop
		if (modeIA) {

			choixCombinaisonIA(Cj, LC, nbTour);

		} else {

			choixCombinaison(Cj, LC);

		}
		
		for (int i=0; i<LC; i+=1) {

			Plateau[nbTour][i] = Cj[i]; // Mise en mémoire de la prop

		}
		reponse rep = calculeReponse(code, Cj, LC);
		Rep[nbTour].bp = rep.bp; Rep[nbTour].bc = rep.bc; // Mise en mémoire de la rep
		printf("===========================\n");
		if (rep.bp == 4) {

			codeTrouve = 1;

		}
		nbTour += 1;
		affichePlateau(nbTour+1);

	}
	
	// Affichage du résultat
	if (codeTrouve && nbTour < NT) {

		printf("\n    Vous avez gagné !\n");

	} else { printf("\n    Vous avez perdu !\n"); }

	printf("\n\n\nAppuyez sur entrer pour fermer la fenêtre...");
    int fin;
    scanf("%c", &fin);
	return 0;

}

//////// Implémentation des fonctions auxiliaires

/* Affiche les valeurs d'un tableau */
void afficheTab(int tab[], int n) {
	for (int i = 0; i < n; i += 1){
		printf("%d ",tab[i]);
	}
}

/* Affiche le plateau de jeu*/
void affichePlateau(int ncoups){
	assert(ncoups <= NT); // Progr. défensive
	for (int i=0; i<ncoups-1; i+=1) {
		afficheTab(Plateau[i], LC);
		printf("      BP:%d BC:%d", Rep[i].bp, Rep[i].bc);
		printf("\n");
	}
}


/* Remplit aléatoirement tab */
void creeCombinaison(int tab[], int n) {

	/*Crée une combinaison aleatoire. Renvoie cette combinaison*/
    for (int i = 0; i<n; i+=1) {

        tab[i] = rand()%(NC+1);
    
    }

}

/* Proposition manuelle */
void choixCombinaison(int tab[], int n) {

	/*Demande a l'utilisateur une combinaison, renvoie cette combinaison*/
	for (int i=0; i<n; i+=1) {

		printf("Entrez un nombre entre 0 et %d :\n", NC-1);
    	int x;
    	scanf("%d", &x);
		tab[i] = x;

	}

}

/* Proposition de l'IA, sachant que ncoups ont été joués */ 
void choixCombinaisonIA(int tab[], int n, int ncoups){
  
	int valide = 0; // La combinaison est-elle valide ?
	while (!valide) {

		// Choisit une combinaison au hasard
		creeCombinaison(tab, LC);
		valide = 1;
		for (int i=0; i<ncoups; i+=1) {	
			reponse res = calculeReponse(tab, Plateau[i], n);
			if ((res.bc != Rep[i].bc) || (res.bp != Rep[i].bp)) {

				valide = 0; // Cette combinaison n'est pas la bonne
				break;

			}
		}

	}
	// On a fini par trouver une combinaison correcte

}

/* Calcule la réponse à donner */
reponse calculeReponse(int code[], int prop[], int n){
	assert(n == LC); // Programmation défensive
	// On travaille sur une copie de code et prop
    // On remplacera par NC tous les pions comptabilisés
	reponse res = {.bc = 0, .bp = 0};
	int code2[LC] = {NC};
	int prop2[LC] = {NC};
	for (int i=0; i<n; i+=1) {

		code2[i] = code[i]; prop2[i] = prop[i];

	}
	// On compte les bien places, et on les remplace par NC dans code2 et prop2
	for (int i=0; i<n; i+=1) {
		if (code2[i] == prop2[i]) { // Bp trouvé

			res.bp += 1;
			code2[i] = NC; prop2[i] = NC;

		}
	}
	// On compte les bonnes couleurs, et on les remplace par NC dans code2 et prop2
	for (int i=0; i<n; i+=1) {
		if (prop2[i] != NC) {
			// On cherche si prop2[i] est dans la liste code2
			for (int j=0; j<n; j+=1) {
				if (code2[j] == prop2[i]) { // Bc trouvé

					res.bc += 1;
					prop2[i] = NC; code2[j] = NC;
					break; // On ne cherche pas plus loin

				}
			}
		}
	}
	assert(res.bp + res.bc <= n); // Programmation défensive
	return res;
}