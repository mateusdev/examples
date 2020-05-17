/*
 * This program takes a 63-char string (C-style) from argv[1] and uses it to
 * fill a base64-like table.
 * 
 * The encode and decode functions, original algorithms of base64, will then utilize
 * this table instead of canonical base64 table.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char table[64];

int generate_table(char * user_table){
	if(strlen(user_table) < 64)
		return -1;

	memcpy(table, user_table, 64);
	return 1;
}

char * b64enc_ascii(char * ptr_str){
	char * tmp = ptr_str;
	int len_ascii = strlen(ptr_str);

	if(len_ascii == 0) return NULL;

	int len = len_ascii;
	len += len % 3 == 0 ? (0) : (3 - (len % 3));
	float temp_len = len;
	temp_len = temp_len + ((float) 1/3 * temp_len);
	len = temp_len;

	char * b64en = (char *) malloc(len + 1);

	int trailing_bytes = (len_ascii % 3 == 0) ? len_ascii % 3 : 3 - len_ascii % 3;
	int index = 0;

	for(int i = 0; i < len_ascii; i += 3){
		int value = *(ptr_str + i);
		value <<= 16;

		if (i + 1 < len_ascii){
			value += (*(ptr_str + i + 1) << 8);
			if (i + 2 < len_ascii){
				value += *(ptr_str + i + 2);
			} else{
				trailing_bytes = 1;
			}

		} else {
			trailing_bytes = 2;
		}
		
		char first_char = value >> 18;
		char second_char = ((value << 6) >> 18) & 0b111111;
		char third_char = ((value << 12) >> 18) & 0b111111;
		char fourth_char = ((value << 18) >> 18) & 0b111111;

		*(b64en + index) = table[first_char];
		*(b64en + index + 1) = table[second_char];
		*(b64en + index + 2) = table[third_char];
		*(b64en + index + 3) = table[fourth_char];
		index += 4;
	}

	*(b64en + len) = '\0';

	for(int i = 0; i < trailing_bytes; i++){
		*(b64en + len - 1 - i) = '=';
	}
	
	return b64en;
}

int index_of(char * array, char value){
	for(int i = 0; i < strlen(array); i++){
		if(array[i] == value)
			return i;
	}
	
	return 0;
}

int qt_trailing(char * b64en){
	int trailing = 0;
	for(int i = 0; i < strlen(b64en); i++){
		if(b64en[i] == '=')
			trailing++;
	}

	return trailing;
}

char * b64dec_ascii(char * ptr_b64str){
	int len = strlen(ptr_b64str);
	
	if(len == 0) return NULL;
	
	int qt_trailings = qt_trailing(ptr_b64str);
	char * ascii_str = (char *) malloc(len - (len / 4) - qt_trailings + 1);
	int index = 0;

	for(int i = 0; i < len; i += 4){
		int string_bytes = 0;

		for(int j = i; j < i + 4; j++){
			string_bytes += index_of(table, *(ptr_b64str + j));
			
			if(j < i + 3)
				string_bytes <<= 6;
		}

		char c1 = (string_bytes  >> 16) & 0xff;
		char c2 = (string_bytes >> 8) & 0xff;
		char c3 = (string_bytes & 0xff);
		
		*(ascii_str + index) = c1;
		*(ascii_str + index + 1) = c2;
		*(ascii_str + index + 2) = c3;
		index += 3;
	}

	*(ascii_str + len - (len / 4) - qt_trailings) = '\0';
	return ascii_str;
}

void b64_free(char * ptr_generated){
	free(ptr_generated);
}

int main(int argc, char **argv){
	if(argc < 2){
		printf("USAGE: %s <table>\n", argv[0]);
		return 1;
	}

	if(generate_table(argv[1]) == -1){
		printf("Please provide a table with 63 chars\n");
		return 1;
	}
	
	char input[64];
	scanf("%64s", input);

	char *en = b64enc_ascii(input);
	printf("%s\n", en);

	char *de = b64dec_ascii(en);
	printf("%s\n", de);

	b64_free(en);
	b64_free(de);
}
