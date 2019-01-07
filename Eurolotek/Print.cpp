#define _CRT_SECURE_NO_WARNINGS

#include "Print.h"
#include "stdafx.h"


void PrintSex(  Uczestnik* p );
void PrintDate(  Uczestnik* p );
void PrintBets(  Uczestnik* p );

void PrintResults( int* pNumsAll5, int* pNumsAll2, Uczestnik** pDraws, int nDrawsNo )
{


	for ( int i = 0; i < nDrawsNo; i++ )
	{
		Uczestnik* pPlayer = *pDraws++;
		printf (  "%s %s    ", pPlayer->nazwisko, pPlayer->imie );
		PrintSex(  pPlayer );
		PrintDate (  pPlayer );
		printf(  "\n" );

		SWIFT* s1 = &( pPlayer->konto.swift );
		for ( int i = 0; i < SWIFT_INSTYTUCJA_DLUGOSC; i++ )
			printf(  "%c", s1->instytucja[i] );
		printf(  " " );

		for ( int i = 0; i < SWIFT_LOKALIZACJA_DLUGOSC; i++ )
			printf(  "%c", s1->lokalizacja[i] );
		printf(  " " );

		for ( int i = 0; i < SWIFT_ODDZIAL_DLUGOSC; i++ )
			printf(  "%c", s1->oddzial[i] );
		printf(  "  " );

		IBAN* i1 = &( pPlayer->konto.iban );
		for ( int i = 0; i < IBAN_KRAJ_DLUGOSC; i++ )
			printf(  "%c", i1->kraj[i] );
		printf(  " " );

		for ( int i = 0; i < IBAN_SUMA_KONTROLNA_DLUGOSC; i++ )
			printf(  "%c", i1->suma_kontrolna[i] );
		printf(  " " );

		for ( int i = 0; i < IBAN_ID_BANK_DLUGOSC; i++ )
			printf(  "%c", i1->ID_bank[i] );
		printf(  " " );

		for ( int i = 0; i < IBAN_ID_KLIENTA_DLUGOSC; i++ )
			printf(  "%c", i1->ID_klient[i] );
		printf(  " " );

		printf(  "  " );

		for ( int i = 0; i < PESEL_DLUGOSC; i++ )
			printf(  "%c", pPlayer->pesel[i] );

		printf(  "\n----------------------------------------------------------------------------------------------------\n" );

		PrintBets (  pPlayer );
	}
		
		printf(  "========================================\n              STATYSTYKI\n========================================\n" );

	
		for ( int i = 0; i < LOTTO; i++ )
		{
			printf(  "%d: %d\t", i + 1, *pNumsAll5++ ); //liczba ile razy, statystyki drukujemy
			if ( ( ( i + 1 ) % 5 ) == 0 )
				printf(  "\n" );
		}

		printf(  "\n\n" );

		for ( int i = 0; i < EXTRALOTTO; i++ )
		{
			printf(  "%d: %d\t", i + 1, *pNumsAll2++ ); //liczba ile razy
			if ( ( ( i + 1 ) % 5 ) == 0 )
				printf(  "\n" );
		}

}


void PrintSex(  Uczestnik* p )
{
	switch ( p->plec)
	{
	case MALE: printf (  "*Facet* \t" ); break;
	case FEMALE: printf (  "*Panienka* \t" ); break;
	}
}

void PrintDate( Uczestnik* p )
{
	switch ( p->data.dzien_tygodnia )
	{
	case MON: printf (  "Mon, " ); break;
	case TUE: printf (  "Tue, " ); break;
	case WED: printf (  "Wed, " ); break;
	case THU: printf (  "Thu, " ); break;
	case FRI: printf (  "Fri, " ); break;
	case SAT: printf (  "Sat, " ); break;
	case SUN: printf (  "Sun, " ); break;
	}
	printf (  "%d/", p->data.dzien );

	switch ( p->data.miesiac )
	{
	case JAN: printf(  "Jan/" ); break;
	case FEB: printf(  "Feb/" ); break;
	case MAR: printf(  "Mar/" ); break;
	case APR : printf(  "Apr/" ); break;
	case MAY: printf(  "May/" ); break;
	case JUN: printf(  "Jun/" ); break;
	case JUL : printf(  "Jul/" ); break;
	case AUG: printf(  "Aug/" ); break;
	case SEP: printf(  "Sep/" ); break;
	case OCT: printf(  "Oct/" ); break;
	case NOV: printf(  "Nov/" ); break;
	case DEC: printf(  "Dec/" ); break;
	}

	printf (  "%d", p->data.rok);
}

void PrintBets(  Uczestnik* p )
{
	Zaklady* tab_zaklady = p->zaklady;

	for ( int i = 0; i < p->zaklad_ilosc; i++ )
	{
		int* z5 = tab_zaklady->Five;
		int* z2 = tab_zaklady->Two;
		for ( int j = 0; j < POPRAWNE; j++ )
			printf (  "%d\t", *z5++ );

		printf (  "|\t" );

		for ( int k = 0; k < EXTRAPOPRAWNE; k++ )
			printf (  "%d   ", *z2++ );

		printf (  "\n" );
		tab_zaklady++;
	}
	printf (  "\n" );
}

