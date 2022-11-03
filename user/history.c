#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void readHis(int hf, int num){
		int count = 0, length = 0;
		char output[512]; //size of historyLog isn't known - lazy solution to assign a lot of memory
		while(read(hf, output, sizeof(output))) {}
		//historyLog is written in reverse order - we need to keep track of how many lines there are so we can read it in reverse order.
		if(num != 0){
				for(int i = 0; i < sizeof(output); i++){
						if(output[i] == '\n'){
								length++; // counting lines
						}
				 }
		}
		//loop through and only print if we are reading normally or if its in the bottom argv[1] lines
		for(int i = 0; i < sizeof(output); i++){
				if(num == 0 || count >= length - num){
						printf("%c", output[i]);
				}
				if(output[i] == '\n'){
						count++;
				}
		}
}

int main(int argc, char *argv[])
{
	int hl;
	if((hl = open("historyLog", 0)) < 0){ //check we can open the file
			fprintf(1, "cannot open historyLog\n");
			exit(1);
	}
	if (argc == 1){ //read normally
			readHis(hl, 0);
	}
	else{ //read a set number of lines - starting at the bottom
			readHis(hl, atoi(argv[1]));
	}
	close(hl);
	return 0;
}
