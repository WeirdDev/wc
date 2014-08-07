#include <malloc.h>
#include "ll.h"

plinkedlist ll_new() {
	/* alokuj a vynuluj novú štruktúru pre spájaný zoznam */
	return (plinkedlist)calloc(sizeof(linkedlist), 1);
}
void ll_destroy(plinkedlist ll) {
	pll_entry tmp, curr = ll->first;
	
	//pokým ešte nie je koniec zoznamu
	while(curr!=NULL) {
		//ulož si nasledujúcu položku zoznamu
		tmp = curr->next;
		
		//dealokuj túto položku
		free(curr);
		
		//... a pokračuj ďalšiou položkou
		curr = tmp;
	}
	
	//nakoniec uvoľni aj samotnú štruktúru zoznamu
	free(ll);
}

int ll_count(plinkedlist ll) {
	int i = 0;
	pll_entry curr = ll->first;
	
	//sme na konci
	while(curr!=NULL) {
		//...nie
		curr = curr->next;
		
		//tak pridaj
		i++;
	}
	
	return i;	
}
void * ll_get(plinkedlist ll, int index) {
	int i = 0;
	pll_entry curr = ll->first;
	
	//pokým niesme na konci
	while(curr!=NULL) {
	
		//ak sme našli položku ktorú hľadáme
		if(i==index)
			//môžeme vrátiť pointer na jej dáta
			return curr->data;
		
		//inak ideme ďalej
		curr = curr->next;
		i++;
	}
	
	//našli sme koniec zoznamu, no nedosiahli sme požadovaný index
	//vraciame neplatnú hodnotu (0xffffffff na 32-bitových systémoch)
	return (void*)-1;
}
int ll_set(plinkedlist ll, int index, void * data) {
	int i = 0;
	pll_entry curr = ll->first;
	
	//ešte niesme na konci?
	while(curr!=NULL) {
		//je toto položka ktorú cheme zmeniť?
		if(i==index) {
			//zmeňme ju
			curr->data = data;
			
			//vráťme index
			return index;
		}
		
		curr = curr->next;
		i++;
	}
	
	//nedosiahli sme potrebný index
	return (int)-1;
}

void ll_add(plinkedlist ll, void * data) {
	//alokuj novú položku, ulož adresu na ňu do dočasnej premennej
	pll_entry tmp = (pll_entry)malloc(sizeof(ll_entry));
	
	//nastav dáta položky
	tmp->data = data;
	//predošlým elementom položky na konci zoznamu je bývalý koniec :)
	tmp->prev = ll->last;
	//zoznam ukončíme nulou, NULL
	tmp->next = NULL;
	
	//bývalá posledná položka musí ukazovať na novú
	ll->last->next = tmp;
	//základná štruktúra zoznamu musí tiež ukazovať na novú poslednú položku
	ll->last = tmp;
}
int ll_remove(plinkedlist ll, int index) {
	int i = 0;
	pll_entry curr = ll->first;
	
	//pokým niesme na konci
	while(curr!=NULL) {
		//našli sme index ktorý hľadáme?
		if(i==index) {
			//ak áno, nasledujúce položka predošlej položky je nasledujúce položka aktuálnej položky
			curr->prev->next = curr->next;
			
			//predošlá položka nasledujúcej položky je predošlá položka aktuálnej položky
			curr->next->prev = curr->prev;
			
			//uvoľni aktuálnu položku
			free(curr);
			
			//vráť nezáporný index
			return index;
		}
		
		curr = curr->next;
		i++;
	}
	
	//v prípade neúspechu vráť zápornú hodnotu
	return (int)-1;
}

void ll_push(plinkedlist ll, void * data) {
	ll_add(ll, data);
}
void * ll_pop(plinkedlist ll) {
	if(ll->last==NULL)
		return NULL;
		
	void * ret = ll->last->data;
	if(ll->last->prev != NULL) {
		ll->last->prev->next = NULL;
		pll_entry last = ll->last;
		ll->last = ll->last->prev;
		free(last);
	}
	
	return ret;
}
