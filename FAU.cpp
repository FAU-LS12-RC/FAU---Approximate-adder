#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <math.h>

using namespace std;

#define N 8
#define M 4
#define P 2

size_t FAU_adder(size_t a, size_t b)
{
    const size_t mask_msp    = ((1ul << abs(N - M)) - 1) << M;  // 1111 0000
    const size_t mask_lsp    =  (1ul << M) - 1;                 // 0000 1111
    const size_t mask_shared = ((1ul << P) - 1) << (M - P);     // 0000 1100
    
    size_t s = 0;
    
    size_t MSP    = (a & mask_msp)    + (b & mask_msp);
    size_t LSP    = (a & mask_lsp)    + (b & mask_lsp);
    size_t shared = (a & mask_shared) + (b & mask_shared);
            
    if(shared & (1ul << M)) 	// Carry prediction
        MSP = MSP + (1ul << M);
    else if(LSP & (1ul << M))
        LSP = mask_lsp;         // Error magnitude reduction
            
    return (MSP & (mask_msp | (1ul << N))) | (LSP & mask_lsp);
}

size_t approximate_adder(size_t a, size_t b)
{
    //Your code goes here: 
    
    
    return a + b;
}

int main (void)
{
    float MED_FAU = 0;
    float MED_APP = 0;

    float ER_FAU  = 0;
    float ER_APP  = 0;
    
    size_t counter_FAU = 1;
    size_t counter_APP = 1;
    
    for(size_t a = 0; a < (1ul << N); a++)
    {
        for(size_t b = 0; b < (1ul << N); b++)
        {
            int16_t error = FAU_adder(a, b) - (a + b);
            
            MED_FAU = MED_FAU + ( abs(error)      - MED_FAU) / counter_FAU;
            ER_FAU  = ER_FAU  + ((abs(error) > 0) - ER_FAU ) / counter_FAU++;
            
	    error = approximate_adder(a, b) - (a + b);
            
	    MED_APP = MED_APP + ( abs(error)      - MED_APP) / counter_APP;
            ER_APP  = ER_APP  + ((abs(error) > 0) - ER_APP ) / counter_APP++;
            
            //cout << endl << " " << a << " + " << b << " = " << a + b << " : " << FAU_adder(a, b);
        }
    }
    
    cout << endl << " MED FAU = " << MED_FAU;
    cout << endl << " ER  FAU = " << ER_FAU;
    
    cout << endl;
    
    cout << endl << " MED APP = " << MED_APP;
    cout << endl << " ER  APP = " << ER_APP;
    
    cout << endl << endl;
    
    return 0;
}
