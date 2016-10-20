#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdint.h>
using namespace std;
#endif /* __PROGTEST__ */

int fibValues[] = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269}; // 30 hodnot

void swapEl(int indA, int indB, bool *fibString)
{
    bool c;
    c=fibString[indA];
    fibString[indA]=fibString[indB];
    fibString[indB]=c;
}

void swapEl2(int indA, int indB, bool *utfString)
{
    bool c;
    c=utfString[indA];
    utfString[indA]=utfString[indB];
    utfString[indB]=c;
}

bool intToFib(int a, int * cur, bool *fibString)
{
    int current;
    int length=0;
    a+=1;
    bool flag=0;

    for(int i=29; i>=0; i--)
    {
        current=*cur+i;
        if(current>999900) return false;

        if (a>=fibValues[i])
        {
            if (flag==0)
            {
                fibString[current+1]=1;
                length=current-*cur+1;
            }
            flag=1;
            fibString[current]=1;
            a-=fibValues[i];
        }
        else if (flag!=0)
        {
            fibString[current]=0;
        }
    }

    *cur+=length+1;
    return true;
}

int power(int a, int n)
{
    if (n==0) return 1;
    int c=a;
    for(; n>1; n--)
    {
        c*=a;
    }
    return c;
}

bool getFib(int * arrSize, const char * inFile, uint32_t * decimal, bool * utfString, int *s_utfString)
{
    ifstream fibin(inFile, ifstream::in | ifstream::binary);

    if (!fibin) return false;

    char c;
    int current=0;
    int noB=0;
    int deccounter=0;

    while (fibin.get(c))
    {
        if(current>999900) return false;
        utfString[current] = ((c >> 7) & 1);
        utfString[current+1] = ((c >> 6) & 1);
        utfString[current+2] = ((c >> 5) & 1);
        utfString[current+3] = ((c >> 4) & 1);
        utfString[current+4] = ((c >> 3) & 1);
        utfString[current+5] = ((c >> 2) & 1);
        utfString[current+6] = ((c >> 1) & 1);
        utfString[current+7] = ((c >> 0) & 1);
        current+=8;
    }


    for(int i=0; i<current; i+=8) // otoceni B
        {
            swapEl2(i, i+7, utfString);
            swapEl2(i+1, i+6, utfString);
            swapEl2(i+2, i+5, utfString);
            swapEl2(i+3, i+4, utfString);
        }


    for(int i=current-1; i>0; i--)
    {
        if (utfString[i]==1 && utfString[i-1]!=1)
            return false;
        else if(utfString[i]==1 && utfString[i-1]==1)
            break;
    }


    /*for(int i=0; i<current; i++)
        {
            cout << utfString[i];
            //if (i%8==7 && i!=0) cout << endl;
        }*/

    bool check=1;
    decimal[0]=0;
    for (int i=0; i<current; i++) // Prevod do dekadicke soustavy
    {
        if(i!=0 && utfString[i]==1 && utfString[i-1]==1 && check==1)
        {
            noB=0;
            deccounter++;
            if(deccounter>99990) return false;
            decimal[deccounter]=0;
            check=0;
            continue;
        }
        check=1;
        decimal[deccounter]+=utfString[i]*fibValues[noB];
        noB++;
        if (noB>29) return false;
    }


    *arrSize=deccounter;
    fibin.close();
    return true;
}

bool getUtf8(int *arrSize, const char * inFile, uint32_t * decimal)
{
    ifstream infile(inFile, ifstream::in | ifstream::binary);
    if (!infile) {return false;}
    int deccounter=0;
    char c=0;
    while(infile.get(c))
    {



        if (((c >> 7) & 1) == 0 ) // 1B cislo
        {
             decimal[deccounter]=0;
             if(((c >> 6) & 1) == 1) decimal[deccounter]+=64;
             if(((c >> 5) & 1) == 1) decimal[deccounter]+=32;
             if(((c >> 4) & 1) == 1) decimal[deccounter]+=16;
             if(((c >> 3) & 1) == 1) decimal[deccounter]+=8;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=4;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=2;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=1;
             if(decimal[deccounter]>1114111) return false;
             deccounter++;
             if(deccounter>99990) return false;
        }
        else if ((((c >> 7) & 1) == 1) && (((c >> 6) & 1) == 1) && (((c >> 5) & 1) == 0) )   // 2B cislo
        {
            decimal[deccounter]=0;
             if(((c >> 4) & 1) == 1) decimal[deccounter]+=1024;
             if(((c >> 3) & 1) == 1) decimal[deccounter]+=512;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=256;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=128;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=64;
             if(!infile.get(c)) return false;

             if ((((c >> 7) & 1) != 1) || (((c >> 6) & 1) != 0)) return false; // Podminka mezilehleho B

             if(((c >> 5) & 1) == 1) decimal[deccounter]+=32;
             if(((c >> 4) & 1) == 1) decimal[deccounter]+=16;
             if(((c >> 3) & 1) == 1) decimal[deccounter]+=8;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=4;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=2;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=1;
             if(decimal[deccounter]>1114111) return false;
             if(deccounter>99990) return false;
             deccounter++;
        }
        else if ((((c >> 7) & 1) == 1) && (((c >> 6) & 1) == 1) && (((c >> 5) & 1) == 1) && (((c >> 4) & 1) == 0) )   // 3B cislo
        {
            decimal[deccounter]=0;
             if(((c >> 3) & 1) == 1) decimal[deccounter]+=32768;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=16384;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=8192;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=4096;
             if(!infile.get(c)) return false;

             if ((((c >> 7) & 1) != 1) || (((c >> 6) & 1) != 0)) return false; // Podminka mezilehleho B

             if(((c >> 5) & 1) == 1) decimal[deccounter]+=2048;
             if(((c >> 4) & 1) == 1) decimal[deccounter]+=1024;
             if(((c >> 3) & 1) == 1) decimal[deccounter]+=512;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=256;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=128;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=64;

             if(!infile.get(c)) return false;

             if ((((c >> 7) & 1) != 1) || (((c >> 6) & 1) != 0)) return false; // Podminka mezilehleho B

             if(((c >> 5) & 1) == 1) decimal[deccounter]+=32;
             if(((c >> 4) & 1) == 1) decimal[deccounter]+=16;
             if(((c >> 3) & 1) == 1) decimal[deccounter]+=8;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=4;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=2;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=1;
             if(decimal[deccounter]>1114111) return false;
             if(deccounter>99990) return false;
             deccounter++;
        }
        else if ((((c >> 7) & 1) == 1) && (((c >> 6) & 1) == 1) && (((c >> 5) & 1) == 1) && (((c >> 4) & 1) == 1) && (((c >> 3) & 1) == 0) )   // 4B cislo
        {
            decimal[deccounter]=0;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=1048576;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=524288;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=262144;
             if(!infile.get(c)) return false;

             if ((((c >> 7) & 1) != 1) || (((c >> 6) & 1) != 0)) return false; // Podminka mezilehleho B
             if(((c >> 5) & 1) == 1) decimal[deccounter]+=131072;
             if(((c >> 4) & 1) == 1) decimal[deccounter]+=65536;
             if(((c >> 3) & 1) == 1) decimal[deccounter]+=32768;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=16384;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=8192;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=4096;
            if(!infile.get(c)) return false;

             if ((((c >> 7) & 1) != 1) || (((c >> 6) & 1) != 0)) return false; // Podminka mezilehleho B
             if(((c >> 5) & 1) == 1) decimal[deccounter]+=2048;
             if(((c >> 4) & 1) == 1) decimal[deccounter]+=1024;
             if(((c >> 3) & 1) == 1) decimal[deccounter]+=512;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=256;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=128;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=64;
            if(!infile.get(c)) return false;

             if ((((c >> 7) & 1) != 1) || (((c >> 6) & 1) != 0)) return false; // Podminka mezilehleho B

             if(((c >> 5) & 1) == 1) decimal[deccounter]+=32;
             if(((c >> 4) & 1) == 1) decimal[deccounter]+=16;
             if(((c >> 3) & 1) == 1) decimal[deccounter]+=8;
             if(((c >> 2) & 1) == 1) decimal[deccounter]+=4;
             if(((c >> 1) & 1) == 1) decimal[deccounter]+=2;
             if(((c >> 0) & 1) == 1) decimal[deccounter]+=1;
             if(decimal[deccounter]>1114111) return false;
             if(deccounter>99990) return false;
             deccounter++;
        }
        else return false;

    }
/*
for(int i=0; i<deccounter; i++)
    cout << decimal[i] << endl;
*/

    *arrSize=deccounter;
    infile.close();
    return true;
}


bool Utf8ToFibonacci ( const char * inFile, const char * outFile )
 {
    uint32_t *arr = (uint32_t*)calloc(100000,sizeof(uint32_t));
    bool *fibString = (bool*)calloc(1000000,sizeof(bool));
    int arrSize=0;
    int cur = 0;

    if(!getUtf8(&arrSize, inFile, arr))
    {
        free(arr);
        free(fibString);
        return false;
    }


    for (int i=0; i<arrSize; i++)
        {
            if(!intToFib(arr[i], &cur, fibString)) return false;
        }


    while(cur%8!=0) // Kolik bude vysledne B?
        {
            cur++;
            fibString[cur]=0;
        }

    for(int i=0; i<cur; i+=8)
        {
            swapEl(i, i+7, fibString);
            swapEl(i+1, i+6, fibString);
            swapEl(i+2, i+5, fibString);
            swapEl(i+3, i+4, fibString);
        }

char c=0;



ofstream ofile(outFile, ios::binary | ios::out);

    for(int z=0; z<cur; z+=8)
        {
            c=0;
            if(fibString[z]==1) c+=128;
            if(fibString[z+1]==1) c+=64;
            if(fibString[z+2]==1) c+=32;
            if(fibString[z+3]==1) c+=16;
            if(fibString[z+4]==1) c+=8;
            if(fibString[z+5]==1) c+=4;
            if(fibString[z+6]==1) c+=2;
            if(fibString[z+7]==1) c+=1;
            if(!ofile.put(c)) return false;
        }
ofile.close();
free(arr);
free(fibString);

    return true;
 }


bool FibonacciToUtf8 ( const char * inFile, const char * outFile )
 {
    uint32_t *arr = (uint32_t*)malloc(100000*sizeof(uint32_t));
    int arrSize;
    bool *utfString = (bool*)malloc(1000000*sizeof(bool));
    int s_utfString=40;
    if(!getFib(&arrSize, inFile, arr, utfString, &s_utfString))
    {
        free(arr);
        free(utfString);
        return false;
    }
    for(int i=0; i<arrSize; i++)
        arr[i]--;

ofstream ofile(outFile, ios::binary | ios::out);

char c=0;
int pom=0;
    for (int i=0; i<arrSize; i++) // kazdy prvek z pole prevadime
    {
        c=0;
        if(arr[i]>1114111) return false;
        pom=arr[i];
        //cout << arr[i] << endl;
        if(arr[i]<128)   // 1B cislo
            {
                c=arr[i];
                if(!ofile.put(c)) return false;
            }
        else if(arr[i]>127 && arr[i]<2048)   // 2B cislo
            {
                c = c | 192; // nastavi horni dva bity na 1
                if(pom>=1024){c = c | 16; pom-=1024;}
                if(pom>=512){c = c | 8; pom-=512;}
                if(pom>=256){c = c | 4;pom-=256;}
                if(pom>=128){c = c | 2;pom-=128;}
                if(pom>=64){c = c | 1;pom-=64;}
                if(!ofile.put(c)) return false;
                // ***
                c=0;
                c = c | 128;

                if(pom>=32){c = c | 32;pom-=32;}
                if(pom>=16){c = c | 16;pom-=16;}
                if(pom>=8){c = c | 8;pom-=8;}
                if(pom>=4){c = c | 4;pom-=4;}
                if(pom>=2){c = c | 2;pom-=2;}
                if(pom>=1){c = c | 1;pom-=1;}

                if(!ofile.put(c)) return false;
            }
        else if(arr[i]>2047 && arr[i]<65536)     // 3B cislo
            {
                c=0;
                c = c | 224; // nastavi horni tri bity na 1
                if(pom>=32768){c = c | 8; pom-=32768;}
                if(pom>=16384){c = c | 4; pom-=16384;}
                if(pom>=8192){c = c | 2;pom-=8192;}
                if(pom>=4096){c = c | 1;pom-=4096;}
                if(!ofile.put(c)) return false;
                // ***
                c=0;
                c = c | 128;

                if(pom>=2048){c = c | 32;pom-=2048;}
                if(pom>=1024){c = c | 16;pom-=1024;}
                if(pom>=512){c = c | 8;pom-=512;}
                if(pom>=256){c = c | 4;pom-=256;}
                if(pom>=128){c = c | 2;pom-=128;}
                if(pom>=64){c = c | 1;pom-=64;}

                if(!ofile.put(c)) return false;

                c=0;
                c = c | 128;
                if(pom>=32){c = c | 32;pom-=32;}
                if(pom>=16){c = c | 16;pom-=16;}
                if(pom>=8){c = c | 8;pom-=8;}
                if(pom>=4){c = c | 4;pom-=4;}
                if(pom>=2){c = c | 2;pom-=2;}
                if(pom>=1){c = c | 1;pom-=1;}

                if(!ofile.put(c)) return false;
            }
        else if(arr[i]>65535 && arr[i]<1114112)  // 4B cislo
            {
                c=0;
                c = c | 240;
                if(pom>=1048576){c = c | 4; pom-=1048576;}
                if(pom>=524288){c = c | 2; pom-=524288;}
                if(pom>=262144){c = c | 1;pom-=262144;}
                if(!ofile.put(c)) return false;

                c=0;
                c = c | 128;
                if(pom>=131072){c = c | 32;pom-=131072;}
                if(pom>=65536){c = c | 16;pom-=65536;}
                if(pom>=32768){c = c | 8;pom-=32768;}
                if(pom>=16384){c = c | 4;pom-=16384;}
                if(pom>=8192){c = c | 2;pom-=8192;}
                if(pom>=4096){c = c | 1;pom-=4096;}

                if(!ofile.put(c)) return false;

                c=0;
                c = c | 128;
                if(pom>=2048){c = c | 32;pom-=2048;}
                if(pom>=1024){c = c | 16;pom-=1024;}
                if(pom>=512){c = c | 8;pom-=512;}
                if(pom>=256){c = c | 4;pom-=256;}
                if(pom>=128){c = c | 2;pom-=128;}
                if(pom>=64){c = c | 1;pom-=64;}

                if(!ofile.put(c)) return false;

                c=0;
                c = c | 128;
                if(pom>=32){c = c | 32;pom-=32;}
                if(pom>=16){c = c | 16;pom-=16;}
                if(pom>=8){c = c | 8;pom-=8;}
                if(pom>=4){c = c | 4;pom-=4;}
                if(pom>=2){c = c | 2;pom-=2;}
                if(pom>=1){c = c | 1;pom-=1;}

                if(!ofile.put(c)) return false;

            }
        else return false;

    }

    ofile.close();
    free(arr);
    free(utfString);
    return true;
 }

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
 {

    //if(!Utf8ToFibonacci(0, 0)) cout << "False";
    //if(!FibonacciToUtf8(0, 0)) cout << "FF";

   return 0;
 }
#endif /* __PROGTEST__ */
