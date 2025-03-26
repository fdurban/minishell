#include "../minishell.h"

int main(void) {
    char *input;
	char	*username;
	// char	*hostname;
	// char	*currentpwd;

	username = getenv("USER");
	//currentpwd = getenv("PWD");

    while (1) {
        // Leer la línea de entrada
        input = readline(username);

        // Si el usuario escribe "listar", lo corregimos a "ls"
        if (strcmp(input, "listar") == 0) {
            rl_replace_line("ls", 1);
            rl_redisplay(); // Actualiza la línea en pantalla
            printf("\nAutocorrección aplicada: 'listar' -> 'ls'\n");
        }

        // Agregamos el comando al historial (si no está vacío)
        if (*input) add_history(input);

        // Salir con "exit"
        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        free(input);
    }

    return 0;
}
