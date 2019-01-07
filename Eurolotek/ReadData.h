#pragma once

#include "defs.h"

int  ReadData( Uczestnik*** pAllDraws, const char* nazwa_pliku );
void FreeMem( Uczestnik** pTab, int nDrawsNo );

void HeapCheck( );