#include <stdio.h>
#include <stdlib.h>

#define MAX_DIM 20
#define NOMEFILE "vestiti.txt"
#define NOMEFILE_TOT "totali.txt"

typedef struct v1{
	int ID,TG,QNT;
	float PRZ;
}Vestito;

typedef struct v2{
	Vestito vestito[MAX_DIM];
	int count; //numero di vestiti letti da file
}Vestiti;

int leggi(char* nomefile, Vestiti* v);

void estrai(int TG, Vestiti v_letti, Vestiti* v_taglia);

int quanti(Vestiti v);

float prezzo(Vestiti v);

int scrivi(char* nomefile_tot, Vestiti* letti);

int trova_doppione(int TG, int doppi[]);


int main(){
	Vestiti vestiti_letti;

	leggi(NOMEFILE, &vestiti_letti) ? printf("\nFile letto correttamente") : printf("\nQualcosa e' andato storto!");

	scrivi(NOMEFILE_TOT, &vestiti_letti) ? printf("\nFile 'totali.txt' creato con successo!") : printf("\nQualcosa e' andato storto!");

	return 0;
}


/*
* la funzione "leggi" prende come parametri il nome del file contenente 
* i dati dei vestiti e l'indirizzo della struttura da riempire.
* Sucessivamente legge dal file e memorizza i dati nella struct.
* Restituisce 1 se l'operazione e' andata a buon fine, 0 in caso di errore.
*/
int leggi(char* nomefile, Vestiti* v){
	FILE* fp = fopen(nomefile, "r");

	if(!fp){
        return 0; // ERROR
	}

    (*v).count = 0; // prima di riempire, inizializzo il contatore a 0
    while(!feof(fp)){
        fscanf(fp,"%d %d %d %f",&(*v).vestito[(*v).count].ID,&(*v).vestito[(*v).count].TG,&(*v).vestito[(*v).count].QNT,&(*v).vestito[(*v).count].PRZ);
        (*v).count++;
    }

	fclose(fp);

	return 1; // OK
}

/*
* La funzione "estrai" prende come parametri la taglia scelta,
* una struct 1 piena (v_letti), e una struct 2 vuota (v_taglia) passata per riferimento.
* I vestiti della struct 1 "v_letti" che hanno la taglia passata come parametro
* vengono memorizzati nella struct 2 (v_taglia)
*/
void estrai(int TG, Vestiti v_letti, Vestiti* v_taglia){
	int i;
	(*v_taglia).count = 0;

	for (i = 0; i < v_letti.count; ++i)
	{
		if(v_letti.vestito[i].TG == TG){
			(*v_taglia).vestito[(*v_taglia).count].ID = v_letti.vestito[i].ID;
			(*v_taglia).vestito[(*v_taglia).count].TG = v_letti.vestito[i].TG;
			(*v_taglia).vestito[(*v_taglia).count].QNT = v_letti.vestito[i].QNT;
			(*v_taglia).vestito[(*v_taglia).count].PRZ = v_letti.vestito[i].PRZ;
			(*v_taglia).count++;
		}
	}
}

/*
* La funzione "quanti" restituisce il numero di 
* vestiti presenti nella struct passata come parametro
*/
int quanti(Vestiti v){
	return v.count;
}

/*
* La funzione "prezzo" restituisce il prezzo totale di tutti
* i vestiti presenti nella struct passata come parametro
*/
float prezzo(Vestiti v){
	int i;
	float somma = 0;

	for (i = 0; i < v.count; ++i) somma += v.vestito[i].PRZ;

	return somma;
}

/*
scrivi(char*, struct*)
2 parametri:
	1) nome del nuovo file
	2) struct con vestiti letti da file

Per ogni vestito presente nella struct con taglia 
compresa tra 42 e 54 scrive sul nuovo file una riga con: 
Taglia - Quantita di vestiti di quella taglia - Prezzo totale di quei vestiti

return: int
	0 = ERR
	1 = OK
*/
int scrivi(char* nomefile_tot, Vestiti* letti){
	int i,j = 0;
	int doppi[MAX_DIM]; // array contenente le taglie gia scritte, in modo da non scriverle due volte
	Vestiti estratti;

	FILE* fp = fopen(nomefile_tot, "w");

	if(!fp){
        return 0; //ERROR
	}

	for (i = 0; i < (*letti).count; ++i)
	{
		if( (*letti).vestito[i].TG >= 42 && (*letti).vestito[i].TG <= 54 ){

            if(trova_doppione( (*letti).vestito[i].TG , doppi) == 0){ // 0 = NON ha trovato un doppione

                doppi[j] = (*letti).vestito[i].TG; // salvo le taglie gia controllate in modo da non scrivere due righe uguali
                j++;

                estrai((*letti).vestito[i].TG , (*letti), &estratti);

                fprintf(fp,"\n%d %d %f", (*letti).vestito[i].TG, quanti(estratti), prezzo(estratti) );
            }
		}
	}

	fclose(fp);

	return 1; // andato a buon fine
}

/*
trova_doppione(int,int [])
2 parametri:
	1) la taglia da controllare
	2) l'array contenente le taglie gia scritte su file

controlla l'array per vedere se la taglia passata
come parametro e' gia' stata scritta sul file
*/
int trova_doppione(int TG, int doppi[]){
    int i;
    for(i = 0; i < 20; i++) if(TG == doppi[i]) return 1; //DOPPIONE TROVATO
    return 0; //DOPPIONE NON TROVATO
}
