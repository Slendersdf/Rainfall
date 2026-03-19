#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int input_val;
    char *shell_path;
    uid_t euid;
    gid_t egid;

	// Convert argv[1] into integer
    // Same as: atoi(*(char **)(param_2 + 4))
    input_val = atoi(argv[1]);

	// Check of the condition comparison: 0x1a7 = 423
    if (input_val == 0x1a7) {

		// Shell path
        shell_path = strdup("/bin/sh");

		// Effectives IDs recovering (owner of the files, so level1)
        egid = getegid();
        euid = geteuid();

		// Setting of the privileges (Real, Effective, Saved)
		// We are now officially the owner of the binary
        setresgid(egid, egid, egid);
        setresuid(euid, euid, euid);

		// Shell execution
        char *args[] = {shell_path, NULL};
        execv(shell_path, args);
    }
	else {
		// Error message if args is incorrect, no argc handle like the original binary
        fwrite("No !\n", 1, 5, stderr);
    }
    return 0;
}
