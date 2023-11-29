#ifndef C201_H
#define C201_H
/* Předmět: Algoritmy (IAL) - FIT VUT v Brně
 * Hlavičkový soubor pro c201.c (Jednosměrně vázaný lineární seznam)
 * Vytvořil: Martin Tuček, září 2005
 * Upravil: Kamil Jeřábek, září 2020
 *          Daniel Dolejška, září 2021
 *
 *
 * Tento soubor, prosíme, neupravujte!
 * Please, do not modify this file!
 */

#define TRUE 1
#define FALSE 0

/** Globální proměnná - indikuje, zda operace volala chybu. */
extern int error_flag;
/** Globální proměnná - indikuje, zda byla operace řešena. */
extern int solved;

/** Prvek jednosměrně vázaného seznamu. */
typedef struct ListElement {
	/** Užitečná data. */
    void* data;
	/** Ukazatel na následující prvek seznamu. */
    struct ListElement *nextElement;
} *ListElementPtr;

/** Jednosměrně vázaný seznam. */
typedef struct {
	/** Ukazatel na první prvek seznamu. */
	ListElementPtr firstElement;
	/** Ukazatel na aktuální prvek seznamu. */
    ListElementPtr activeElement;
} List;

void List_Init( List * );

void List_Dispose( List * );

void List_InsertFirst( List *, void* );

void List_First( List * );

void List_GetFirst( List *, void** );

void List_DeleteFirst( List * );

void List_DeleteAfter( List * );

void List_InsertAfter( List *, void*  );

void List_Next( List * );

void List_GetValue( List *, void** );

void List_SetValue( List *, void* );

int List_IsActive( List * );

/* Konec hlavičkového souboru c201.h */
#endif
