#include <iostream>
#include <cstdlib> // For rand(), srand()
#include <ctime>   // For time()
#include <cstdio>
using namespace std;

int main()
{
    int N = 1;
    const int size = 100;

    const char* limit = "Give me a positive (>1): ";
    const char* category = "%d";
    _asm {
    start:
        push limit;
        call printf;

        lea eax, N;
        push eax;
        push category;
        call scanf;
       
        mov ecx, N;
        cmp ecx, 1;
        jle start;
    }

    double array[size];
    double evens[size];
    srand((unsigned)time(NULL));
    _asm {
        mov edi, 0;
        mov ecx, 0;

    l_start:
        inc ecx;
        rdrand eax;
        mov ebx, N;
        mov edx, 0;
        div ebx;
        add edx, 1;
        mov [esp], edx;

        fild dword ptr[esp];
        fstp qword ptr[array + edi * 8];
        inc edi;

        cmp ecx, 100;
        jl l_start;
    }

    int index = 0;
    _asm {
        mov ecx, 0;       //i = 0
        mov ebx, 0;       //index = 0
        mov esi, 100;     //size = 100
        lea edi, evens;   //load base address of evens
        //start of the loop
    loop_start:
        cmp ecx, esi;     //compare i and size
        jge loop_end;     //jump if greater or equal

        fld qword ptr[array + ecx * 8];  //load from array[i]
        sub esp, 8;
        fistp dword ptr [esp];           //turn it to int
        mov eax, [esp];                  //move it to eax
        test eax, 1;                     // test if is even
        jnz not_even;                    // jump if not 0 (not even)

        
        mov [esp], eax;                 //store in evens[index] into esp
        fild dword ptr[esp];            //store from int to double
        fstp qword ptr[edi + ebx * 8];
        inc ebx;                         //index++
        jmp not_even;

    not_even:
        inc ecx;                       //i++
        jmp loop_start;                //jump back to the loop

    loop_end:
        mov index, ebx;                //store the value of index
       
    }
  
    const char* even_nums = "\nEven numbers in the array:\n";
    const char* output = "%.0lf\t";
    const char* space = ",";
    const char* finish = "finish";
    const char* newline = "\n";
    
    _asm {
        push even_nums;
        call printf;

        mov esi, 0;
        mov ecx, index;
    l_begin:
        mov edi, ecx;
        fld evens[esi];
        sub esp, 8;
        fstp qword ptr[esp];
        push output;
        call printf;

        mov ecx, edi;
        add esi, 8;
        loop l_begin;
    
        push finish;
        call printf;
        push newline;
        call printf;
    }

    return 0;
}



