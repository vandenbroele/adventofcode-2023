#include <stdio.h>
#include <string.h>
#define MAX_LEN 512

int get_calibration_value(char *s){
	int i, value;

	for(i = 0; i < strlen(s) && (s[i] <= '0' || '9' < s[i]); i++);
	value = 10 * (s[i] - '0');

	for(i = strlen(s) - 1; 0 <= i && (s[i] <= '0' || '9' < s[i]); i--);
	value += s[i] - '0';

	return value;
}

int main(){
	char s[MAX_LEN];
	int sum;

	sum = 0;
	while(scanf("%s", s) != EOF){
		sum += get_calibration_value(s);
	}

	printf("%d\n", sum);

	return 0;
}
