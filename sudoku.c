#include <stdio.h>
#include <stdlib.h>

int x[100], nine = 9, three = 3, line, column, lineIndex, columnIndex;

int findZero();
int valid(int position, int value);
int solve();

int main()
{
	int i;
	FILE *f_inp, *f_out;
	f_inp = fopen("input.txt", "r");
	if(f_inp == NULL){
		printf("Input file doesn't exist!");
		exit(1);
	}
	for(i = 0; i < 81; i++)
		fscanf(f_inp, "%d", &x[i]);
	fclose(f_inp);
	f_out = fopen("output.txt", "w");
	if(solve() == 1)
		for(i = 0; i < 81; i++){
			if(i%9 == 0 && i > 0)
				fprintf(f_out, "\n");
			fprintf(f_out, "%d ", x[i]);
		}
	else
		fprintf(f_out, "-1");	
	fclose(f_out);
	return 0;
}
asm(
"findZero:;"
	"pushl %ebp;"
	"movl %esp, %ebp;"
	"pushl %edi;"
	
	"movl $-1, %eax;"
	"movl $x, %edi;"
	"xorl %ecx, %ecx;"
	"fZ_for:;"
		"cmp $81, %ecx;"
		"je fZ_exit;"
		
		"cmp $0, (%edi, %ecx, 4);"
		"je fZ_succes;"
		
		"incl %ecx;"
		"jmp fZ_for;"
	"jmp fZ_exit;"
	"fZ_succes:;"
		"movl %ecx, %eax;"
	"fZ_exit:;"
	"popl %edi;"
	"popl %ebp;"
	"ret;"
);

asm(
"valid:;"
	"pushl %ebp;"
	"movl %esp, %ebp;"
	"pushl %edi;"
	"pushl %ebx;"
	
	"movl 8(%ebp), %eax;"  // position
	"movl 12(%ebp), %ebx;" // value
		
	"movl $x, %edi;"
		
	"xorl %edx, %edx;"
	"divl nine;" // %edx = column
	"movl %eax, line;"
	"movl %edx, column;"
		
	"mull nine;" // %eax = start of the position
	"movl %eax, %ecx;"
	"addl $9, %eax;"
	"forLine:;" // check line
		"cmp %eax, %ecx;"
		"je fL_exit;"
			
		"cmp (%edi, %ecx, 4), %ebx;"
		"je notValid;"
			
		"incl %ecx;"
		"jmp forLine;"
	"fL_exit:;"
	
	"movl column, %ecx;"
	"forColumn:;" // check column
		"cmp $80, %ecx;"
		"jg fC_exit;"
		
		"cmp (%edi, %ecx, 4), %ebx;"
		"je notValid;"
		
		"addl $9, %ecx;"
		"jmp forColumn;"
	"fC_exit:;"
	
	// ----------------
	"movl line, %eax;"
	"xorl %edx, %edx;"
	"divl three;"
	"movl line, %eax;"
	"subl %edx, %eax;"
	"mull nine;"
	"movl %eax, line;"
	
	"movl column, %eax;"
	"xorl %edx, %edx;"
	"divl three;"
	"movl column, %eax;" 
	"subl %edx, %eax;" 
	"add line, %eax;"
	// ----------------
	// %eax = (line-line%3)*9+(column-column%3)
	
	"movl $0, lineIndex;"
	"movl %eax, %ecx;"
	"squareLine:;" // check 3x3 square
		"cmp $3, lineIndex;"
		"je exit_sL;"
		
		"movl $0, columnIndex;"
		"squareColumn:;"
			"cmp $3, columnIndex;"
			"je exit_sC;"
			
			"cmp (%edi, %ecx, 4), %ebx;"
			"je notValid;"
			
			"incl %ecx;"
			"incl columnIndex;"
			"jmp squareColumn;"
		"exit_sC:;"
		"addl $6, %ecx;"
		"incl lineIndex;"
		"jmp squareLine;"
	"exit_sL:;"
		
	"movl $1, %eax;"
	"jmp valid_exit;" // true
				
	"notValid:;"
		"xorl %eax, %eax;" // false
	
	"valid_exit:;"
	"popl %ebx;"
	"popl %edi;"
	"popl %ebp;"
	"ret;"
);
asm(
"solve:;"
	"pushl %ebp;"
	"movl %esp, %ebp;"
	"pushl %ebx;"
	"pushl %edi;"
	
	"call findZero;"
	"cmp $-1, %eax;"
	"je solveSuccess;"
	
	"movl %eax, %ebx;" // x[%ebx] = 0
	
	"movl $x, %edi;"
	"movl $-1, %ecx;"
	"back_for:;"
		"cmp $9, %ecx;"
		"je exit_back;"
		
		"incl %ecx;"
		
		"pushl %ecx;" // value
		"pushl %ebx;" // position
		"call valid;"
		"popl %ebx;"
		"popl %ecx;"
		
		"cmp $1, %eax;"
		"je back_valid;"
		
		"jmp back_for;"
		
		"back_valid:;"

			"movl %ecx, (%edi, %ebx, 4);" // x[%ebx] = %ecx"

			"push %ecx;"
			"push %ebx;"
			"call solve;"
			"popl %ebx;"
			"popl %ecx;"
	
			"cmp $1, %eax;"
			"je solveSuccess;"
			
			"movl $0, (%edi, %ebx, 4);" // x[%ebx] = 0

			"jmp back_for;"
	"exit_back:;"
	
	"movl $0, %eax;"
	"jmp solveExit;"
	
	"solveSuccess:;"
		"movl $1, %eax;"
	
	"solveExit:;"
	"popl %edi;"
	"popl %ebx;"
	"popl %ebp;"
	"ret;"	
);

