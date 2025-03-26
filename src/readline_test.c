#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int main() {
	rl_on_new_line();
    char *input = readline("Ingrese un comando: ");
    if (input) {
        printf("Usted ingresó: %s\n", input);
        free(input);  // Liberar la memoria
    }
    return 0;
}