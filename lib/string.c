#include <string.h>

int strcmp(const char *str1, const char *str2) {
	int i = 0;
	while (str1[i] == str2[i]) {
		if (str1[i] == '\0') {
			return 0;
		}
		i++;
	}
	return -1;
}