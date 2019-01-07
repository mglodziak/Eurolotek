#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "defs.h"
#include "ReadData.h"
#include "Print.h"
#include <stdlib.h>
#include <string.h>



void ObliczStatystyki( int* pNumsB, int* pNumsS, Uczestnik** pDraws, int nDrawsNo );
void Sortuj( Uczestnik** pDraws, int nDrawsNo );
int  StrCompare( char * s1, char *s2 );


int main( int argc, char* argv[] )
{
	if ( argc != 3 )
	{
		printf( "Bledna liczba argumentow" );
		return 2;
	}

	Uczestnik** gracze = NULL;

	int liczba_graczy = ReadData( &gracze, argv[1] );
	if ( !liczba_graczy )
	{
		printf ( "Error: No Data Loaded!\n" );
		return 2;
	}

	Sortuj( gracze, liczba_graczy );


	int* pNumsAll2 = ( int* )calloc( 10, sizeof( int ) );
	if ( !pNumsAll2 )
	{
		perror( "error with memory in pNumsAll2" );
		return 0;
	}
	int* pNumsAll5 = ( int* )calloc( 50, sizeof( int ) );
	if ( !pNumsAll5 )
	{
		perror( "error with memory in pNumsAll5" );
		return 0;
	}
	ObliczStatystyki( pNumsAll5, pNumsAll2, gracze, liczba_graczy );

	PrintResults( pNumsAll5, pNumsAll2, gracze, liczba_graczy );



	////ZWALNIANIE PAMIÊCI
	free( pNumsAll5 );
	free( pNumsAll2 );
	FreeMem( gracze, liczba_graczy );

	HeapCheck( );	///HeapCheck(); --   mozna dodac ta funkcje - mozna skopiowac z helpow
	
    return 0;
}

void ObliczStatystyki( int* pNumsAll5, int* pNumsAll2, Uczestnik** pDraws, int nDrawsNo )
{	
	if( nDrawsNo < 1 )
		return;

	int* a5 = pNumsAll5;
	int* a2 = pNumsAll2;
	for( int i = 0; i < nDrawsNo; i++ )
	{
		Uczestnik* Player = *pDraws++;
		Zaklady* tabBets = Player->zaklady;

		for( int j = 0; j < ( Player->zaklad_ilosc ); j++ )
		{
			int* Num5 = tabBets->Five;
			for( int k = 0; k < POPRAWNE; k++ )
			{
				if( ( *Num5 > ( LOTTO + 1 ) ) || ( *Num5 < 0 ) )
				{
					printf( "Podano zla liczbe: %d -> statystyki bêd¹ b³êdne!", *Num5 );
					return;
				}
				a5[*Num5++ - 1]++;
			}

			int* Num2 = tabBets->Two;
			for( int k = 0; k < EXTRAPOPRAWNE; k++ )
			{
				if( ( *Num2 > ( EXTRALOTTO + 1 ) ) || ( *Num2 < 0 ) )
				{
					printf( "Podano zla liczbe: %d -> statystyki bêd¹ b³êdne!", *Num2 );
					return;
				}
				a2[*Num2++ - 1]++;
			}
			tabBets++;
		}
	}

}

void Sortuj( Uczestnik** pDraws, int nDrawsNo )
{
//	Uczestnik** temp = pDraws;
	for ( int i = 0; i < nDrawsNo-1; i++ )
	{
		Uczestnik* min = pDraws[i];
		int x = i;


		for ( int j = i + 1; j < nDrawsNo; j++ )
		{
			if ( ( StrCompare( pDraws[j]->nazwisko, min->nazwisko ) ) <0 )
				{
					min = pDraws[j];
					x = j;
				}
			
		}
		pDraws[x] = pDraws[i];
		pDraws[i] = min;
	}
}
int  StrCompare( char * s1, char *s2 ) // porównujê 2 ³añcuchy funkcjami bibliotecznymi, idê na ³atwiznê.. 
{
	char* str1 = ( char* )calloc( strlen( s1 ) + 1, sizeof( char ) );
	char* str2 = ( char* )calloc( strlen( s2 ) + 1, sizeof( char ) );

	strcpy( str1, s1 );
	//_strupr( str1 ); w tym przypadku np Kevin deHooi wyl¹duje na koñcu.

	strcpy( str2, s2 );
	//( str2 );

	int wynik = strcmp( str1, str2 );

	free( str1 );
	free( str2 );

	return wynik;
}
