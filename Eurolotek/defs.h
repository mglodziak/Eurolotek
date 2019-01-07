//plik na definicje wszystkich sta³ych
#pragma once

#define LOTTO 50
#define POPRAWNE 5
#define EXTRALOTTO 10
#define EXTRAPOPRAWNE 2

#define MAXNAME 31
#define PESEL_DLUGOSC 10
#define PAYBACK_DLUGOSC 10

#define IBAN_SUMA_KONTROLNA_DLUGOSC 2
#define IBAN_ID_BANK_DLUGOSC 8
#define IBAN_ID_KLIENTA_DLUGOSC 16
#define IBAN_KRAJ_DLUGOSC 2

#define SWIFT_INSTYTUCJA_DLUGOSC 4
#define SWIFT_LOKALIZACJA_DLUGOSC 4
#define SWIFT_ODDZIAL_DLUGOSC 3

#define REALLOC_GRACZ 5
#define REALLOC_ZAKLAD 3



typedef enum { MON, TUE, WED, THU, FRI, SAT, SUN } Dzien;
typedef enum { JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC } Miesiac;
typedef enum { MALE, FEMALE } Plec;


typedef struct
{
	char suma_kontrolna[IBAN_SUMA_KONTROLNA_DLUGOSC]; //oddzial, kraj, inne gówno
	char ID_bank[IBAN_ID_BANK_DLUGOSC];
	char ID_klient[IBAN_ID_KLIENTA_DLUGOSC];
	char kraj[IBAN_KRAJ_DLUGOSC];
}IBAN;

typedef struct
{
	char instytucja[SWIFT_INSTYTUCJA_DLUGOSC];
	char lokalizacja[SWIFT_LOKALIZACJA_DLUGOSC];
	char oddzial[SWIFT_ODDZIAL_DLUGOSC];

}SWIFT;

typedef struct
{
	IBAN iban;
	SWIFT swift;
}Dane_konta; //dane konta

typedef struct
{
	int dzien; //numer dnia (1, 3, 5 itp)
	Dzien dzien_tygodnia; // nazwa dnia tygodnia (poniedzia³ek, wtorek..)
	Miesiac miesiac; // styczeñ, kwiecieñ
	int rok;
}Data;

typedef struct
{
	int* Five; // 5 z 50
	int* Two; // 2 z 10
}Zaklady;

typedef struct
{
	char imie[MAXNAME];
	char nazwisko[MAXNAME];
	char pesel[PESEL_DLUGOSC]; // bo pesel ma 11 cyfr
	Data data;
	Plec plec;
	Dane_konta konto;
	Zaklady* zaklady;
	int zaklad_max;
	int zaklad_ilosc;


}Uczestnik;