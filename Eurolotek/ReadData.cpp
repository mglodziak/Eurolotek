#define _CRT_SECURE_NO_WARNINGS
#include "defs.h"
#include "ReadData.h"
#include "stdafx.h"
#include <malloc.h> //potrzebne do heap check
#include "time.h"


int  AllocAddMem( Uczestnik*** pTab, int nCurrSize );
int  AllocAddMemBet( Zaklady** pTab, int nCurrSize );

int  SetSex( Uczestnik* p, char c );
int  SetDate( Uczestnik* p, int d, int m, int r );




int  ReadData( Uczestnik*** pAllDraws, const char* nazwa_pliku )
{
	FILE* fin = fopen( nazwa_pliku, "r" ); //otwieram plik, tylko do odczytu
	if( !fin ) //sprawdzam, czy udalo sie otworzyc poprawnie plik
	{
		printf ( "Error: nie udalo sie otworzyc pliku!!\n" );
		return 0;
	}

	int ilosc_graczy = 0;
	int max_rozmiar = 0; //rozmiar tablicy pAllDraws

	Uczestnik** tmp = *pAllDraws;
	
	char c;
	while ( ( c = fgetc( fin ) ) == '*' || !feof( fin ) ) //od gwiazdki zaczynamy wczytywanie kolejnych ludzi, wczytuj, a¿ trafisz na koniec pliku
	{
		// sprawdzam, czy mam pamiêæ
		if ( ilosc_graczy == max_rozmiar ) //realokuje pamiec jesli nie mam miejsca w tablicy
		{
			max_rozmiar += AllocAddMem( &tmp, ilosc_graczy );
			if ( ilosc_graczy == max_rozmiar )
				break;
		}

		//w tym miejscu ju¿ konkretnie wczytujê dane


		Uczestnik* nowy_gracz = tmp[ilosc_graczy] = ( Uczestnik* )calloc( 1, sizeof( Uczestnik ) ); //alokujê pamiêæ dla nowego gracza 
		if ( !nowy_gracz )
		{
			perror( "Error creating: no memory!" );
			return 2;
		}

		char plec;
		int dzien;
		int miesiac;
		int rok;

		SWIFT* p1 = &( nowy_gracz->konto.swift );
		IBAN* p2 = &( nowy_gracz->konto.iban ); 

		fscanf( fin, "%s %s %c %4c %4c %3c %2c %2c %8c %16c %d/%d/%d %10c \n", //³adujê dane do struktur
			nowy_gracz->nazwisko,
			nowy_gracz->imie,
			&plec,
			
			p1->instytucja,
			p1->lokalizacja,
			p1->oddzial,
			
			p2->kraj,
			p2->suma_kontrolna,
			p2->ID_bank,
			p2->ID_klient,
			
			&dzien,
			&miesiac,
			&rok,
			nowy_gracz->pesel);

		if ( !SetSex( nowy_gracz, plec ) || !SetDate( nowy_gracz, dzien, miesiac, rok ) )
		{
			perror( "Error: SetSex or SetDate!" );
			break;
		}

		nowy_gracz->zaklad_ilosc = 0; // ilosc wczytanych zakladów
		nowy_gracz->zaklad_max = 0;

		
		while ( ( c = fgetc( fin ) ) != '*' && c != EOF )
		{
			if ( nowy_gracz->zaklad_ilosc == nowy_gracz->zaklad_max )
			{
				nowy_gracz->zaklad_max += AllocAddMemBet( &nowy_gracz->zaklady, nowy_gracz->zaklad_ilosc ); //realokacja jeœli nie ma miejsca w tab
				if ( nowy_gracz->zaklad_ilosc == nowy_gracz->zaklad_max ) break;
			}

			ungetc( c, fin ); //kurde, zaœ ten unget siê zapomnia³...

			int* piec = nowy_gracz->zaklady[nowy_gracz->zaklad_ilosc].Five = ( int* )calloc( POPRAWNE, sizeof ( int ) );
			int* dwa = nowy_gracz->zaklady[nowy_gracz->zaklad_ilosc].Two = ( int* )calloc( EXTRAPOPRAWNE, sizeof ( int ) );

			if ( !piec || !dwa )
			{
				printf ( "Error: nie udalo sie zaalokowac pamieci dla poprawnych trafien " );
				return 0;
			}

			for ( int i = 0; i < POPRAWNE; i++ )
				fscanf ( fin, "%d", piec++ );

			fscanf ( fin, "%d %d\n", dwa, dwa + 1 );

			nowy_gracz->zaklad_ilosc++;
		}
		ungetc ( c, fin );
		ilosc_graczy++;
	}

	*pAllDraws = tmp;
	fclose ( fin );
	
	
	return ilosc_graczy;
}



void FreeMem( Uczestnik** pTab, int nDrawsNo )
{
	Uczestnik** p = pTab;

	for( int i = 0; i < nDrawsNo; i++ )
	{
		Uczestnik* tmp = *p++;
		Zaklady* tab_zaklady = tmp->zaklady;
		for( int j = 0; j < tmp->zaklad_max; j++ )
		{
			free ( tab_zaklady[j].Two );
			free ( tab_zaklady[j].Five );
		}
		free ( tmp->zaklady );
		free( tmp );
	}
	free ( pTab );
	pTab = NULL;

}
//------------

void HeapCheck( ) // z helpa
{
	int heapstatus = _heapchk( );
	switch ( heapstatus )
	{
	case _HEAPOK:
		printf( " OK - heap is fine\n" );
		break;
	case _HEAPEMPTY:
		printf( " OK - heap is empty\n" );
		break;
	case _HEAPBADBEGIN:
		printf( "ERROR - bad start of heap\n" );
		break;
	case _HEAPBADNODE:
		printf( "ERROR - bad node in heap\n" );
		break;
	}
}
//------------

int  AllocAddMem( Uczestnik*** pTab, int nCurrSize )
{
	int nowy_rozmiar = REALLOC_GRACZ + nCurrSize;
	Uczestnik **tmp = ( Uczestnik** )realloc( *pTab, nowy_rozmiar* sizeof( Uczestnik* ) );
	if( !tmp )
	{
		printf ( "Error: Brak pamieci przy realokacji gracza!\n" );
		return 0;
	}
	memset( tmp + nCurrSize, 0, REALLOC_ZAKLAD );
	*pTab = tmp;
	return REALLOC_GRACZ;
}
//------------

int  AllocAddMemBet( Zaklady** pTab, int nCurrSize ) //chyba tak to powinno byæ
{
	*pTab = ( Zaklady* )realloc( *pTab, (REALLOC_ZAKLAD + nCurrSize) * sizeof( Zaklady ) );
	if( !pTab )
	{
		printf ( "Error: Brak pamieci przy realokacji zakldow!\n" );
		return 0;
	}
	memset ( *pTab + nCurrSize, 0, REALLOC_ZAKLAD * sizeof( Zaklady ) );
	return REALLOC_ZAKLAD;
}
//------------

int  SetSex( Uczestnik* p, char c )
{
	switch ( c )
	{
	case 'f':
	case 'F': p->plec = FEMALE; break;
	case 'm':
	case 'M': p->plec = MALE; break;

	}
	return 1;
}
//------------

int  SetDate( Uczestnik* p, int d, int m, int r )
{
	if ( d < 1 || d>31 ) //sprawdzam, czy daty s¹ poprawne, jeœli to wychodzê
	{
		printf ( "Bledna data - numer dnia!" );
			return 0;
	}
	if ( m < 1 || m>12 )
	{
		printf ( "Bledna data - numer miesiaca!" );
		return 0;
	}
	if ( r < 1900 || r>2100 )
	{
		printf ( "Bledna data - numer roku!" );
		return 0;
	}

	p->data.dzien = d; //jeœli daty s¹ ok, ustawiam j¹ odpowiednio.
	p->data.rok = r;

	switch ( m )
	{
	case 1: p->data.miesiac = JAN; break;
	case 2: p->data.miesiac = FEB; break;
	case 3: p->data.miesiac = MAR; break;
	case 4: p->data.miesiac = APR; break;
	case 5: p->data.miesiac = MAY; break;
	case 6: p->data.miesiac = JUN; break;
	case 7: p->data.miesiac = JUL; break;
	case 8: p->data.miesiac = AUG; break;
	case 9: p->data.miesiac = SEP; break;
	case 10: p->data.miesiac = OCT; break;
	case 11: p->data.miesiac = NOV; break;
	case 12: p->data.miesiac = DEC; break;
	}

	
	// jak wyliczyc dzien tygodnia??
	
	//-----------------
	struct tm Czas;
	memset( &Czas, 0, sizeof( Czas ) );

	Czas.tm_year = r - 1900; //ta funkcja liczy od 1900 roku
	Czas.tm_mday = d;
	Czas.tm_mon = m - 1; //liczy od 0
							   //tm_wday niedziela to 0
							   //-------
	mktime( &Czas );

	switch ( Czas.tm_wday )
	{
	case 0: p->data.dzien_tygodnia = SUN; break; //niedziela to 0, kolejne dni od jednyki
	case 1: p->data.dzien_tygodnia = MON; break;
	case 2: p->data.dzien_tygodnia = TUE; break;
	case 3: p->data.dzien_tygodnia = WED; break;
	case 4: p->data.dzien_tygodnia = THU; break;
	case 5: p->data.dzien_tygodnia = FRI; break;
	case 6: p->data.dzien_tygodnia = SAT; break;

	}

	return 1;
}