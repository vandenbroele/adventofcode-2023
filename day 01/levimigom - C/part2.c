#include <stdio.h>
#include <string.h>
#define bool unsigned char
#define true 1
#define false 0
#define MAX_LEN 512

char *digits[] = {
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

bool starts_with(char *haystack, char *needle){
	int i;

	for(i = 0; i < strlen(needle); i++){
		if(strlen(haystack) <= i || haystack[i] != needle[i]){
			return false;
		}
	}

	return true;
}

int get_digit(char *s){
	if('0' <= s[0] && s[0] <= '9') {
		return s[0] - '0';
	}

	int i;
	for(i = 0; i < 9; i++){
		if(starts_with(s, digits[i])){
			return i + 1;
		}
	}

	return -1;
}

int get_calibration_value(char *s){
	int i, value;

	for(i = 0; i < strlen(s) && get_digit(s + i) == -1; i++);
	value = 10 * (get_digit(s + i));

	for(i = strlen(s) - 1; 0 <= i && get_digit(s + i) == -1; i--);
	value += get_digit(s + i);

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
