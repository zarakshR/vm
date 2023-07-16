vm: vm.c
	gcc -Wall -Wextra -g -O3 -D_FORTIFY_SOURCE=2 vm.c -o vm
