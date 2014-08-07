#include <malloc.h>
#include "ll.h"

plinkedlist ll_new() {
	/* alokuj a vynuluj novĂş ĹˇtruktĂşru pre spĂˇjanĂ˝ zoznam */
	return (plinkedlist)calloc(sizeof(linkedlist), 1);
}
void ll_destroy(plinkedlist ll) {
	pll_entry tmp, curr = ll->first;
	
	//pokĂ˝m eĹˇte nie je koniec zoznamu
	while(curr!=NULL) {
		//uloĹľ si nasledujĂşcu poloĹľku zoznamu
		tmp = curr->next;
		
		//dealokuj tĂşto poloĹľku
		free(curr);
		
		//... a pokraÄŤuj ÄŹalĹˇiou poloĹľkou
		curr = tmp;
	}
	
	//nakoniec uvoÄľni aj samotnĂş ĹˇtruktĂşru zoznamu
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
	
	//pokĂ˝m niesme na konci
	while(curr!=NULL) {
	
		//ak sme naĹˇli poloĹľku ktorĂş hÄľadĂˇme
		if(i==index)
			//mĂ´Ĺľeme vrĂˇtiĹĄ pointer na jej dĂˇta
			return curr->data;
		
		//inak ideme ÄŹalej
		curr = curr->next;
		i++;
	}
	
	//naĹˇli sme koniec zoznamu, no nedosiahli sme poĹľadovanĂ˝ index
	//vraciame neplatnĂş hodnotu (0xffffffff na 32-bitovĂ˝ch systĂ©moch)
	return (void*)-1;
}
int ll_set(plinkedlist ll, int index, void * data) {
	int i = 0;
	pll_entry curr = ll->first;
	
	//eĹˇte niesme na konci?
	while(curr!=NULL) {
		//je toto poloĹľka ktorĂş cheme zmeniĹĄ?
		if(i==index) {
			//zmeĹ�me ju
			curr->data = data;
			
			//vrĂˇĹĄme index
			return index;
		}
		
		curr = curr->next;
		i++;
	}
	
	//nedosiahli sme potrebnĂ˝ index
	return (int)-1;
}

void ll_add(plinkedlist ll, void * data) {
	//alokuj novĂş poloĹľku, uloĹľ adresu na Ĺ�u do doÄŤasnej premennej
	pll_entry tmp = (pll_entry)malloc(sizeof(ll_entry));
	
	//nastav dĂˇta poloĹľky
	tmp->data = data;
	//predoĹˇlĂ˝m elementom poloĹľky na konci zoznamu je bĂ˝valĂ˝ koniec :)
	tmp->prev = ll->last;
	//zoznam ukonÄŤĂ­me nulou, NULL
	tmp->next = NULL;
	
	//bĂ˝valĂˇ poslednĂˇ poloĹľka musĂ­ ukazovaĹĄ na novĂş
	ll->last->next = tmp;
	//zĂˇkladnĂˇ ĹˇtruktĂşra zoznamu musĂ­ tieĹľ ukazovaĹĄ na novĂş poslednĂş poloĹľku
	ll->last = tmp;
}
int ll_remove(plinkedlist ll, int index) {
	int i = 0;
	pll_entry curr = ll->first;
	
	//pokĂ˝m niesme na konci
	while(curr!=NULL) {
		//naĹˇli sme index ktorĂ˝ hÄľadĂˇme?
		if(i==index) {
			//ak Ăˇno, nasledujĂşce poloĹľka predoĹˇlej poloĹľky je nasledujĂşce poloĹľka aktuĂˇlnej poloĹľky
			curr->prev->next = curr->next;
			
			//predoĹˇlĂˇ poloĹľka nasledujĂşcej poloĹľky je predoĹˇlĂˇ poloĹľka aktuĂˇlnej poloĹľky
			curr->next->prev = curr->prev;
			
			//uvoÄľni aktuĂˇlnu poloĹľku
			free(curr);
			
			//vrĂˇĹĄ nezĂˇpornĂ˝ index
			return index;
		}
		
		curr = curr->next;
		i++;
	}
	
	//v prĂ­pade neĂşspechu vrĂˇĹĄ zĂˇpornĂş hodnotu
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
void * ll_peek(plinkedlist ll) {
	if(ll->last==NULL)
		return NULL;

	return ll->last->data;
}
