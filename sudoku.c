#include <stdio.h>
#include <stdlib.h>

int x[81];




int main()
{
	FILE *f_inp, *f_out;
	f_inp = fopen("input.txt", "r");
	if(f_inp == NULL){
		printf("Input file doesn't exist!");
		exit(1);
	}
	for(int i = 1; i <= 81; i++)
		fscanf(f_inp, "%d", &x[i]);
	fclose(f_inp);
	f_out = fopen("output.txt", "w");
	for(int i = 1; i <= 81; i++){
		fprintf(f_out, "%d ", x[i]);
		if(i%9 == 0)
			fprintf(f_out, "\n");
	}
	return 0;
}
