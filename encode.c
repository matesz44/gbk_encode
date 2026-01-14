#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* gcc -Wall -Werror -Wextra -pedantic -Wmain -std=c89 -o encode encode.c
   echo -en '[TEXT|IP]' | ./encode {h | d | gbk | ia | ih | id | ib}
   echo -en 'AAA' | ./encode d --> 41,41,41, */

void
die(const char *msg)
{
	printf("%s {h(ex) | d(ec) | gbk | ia(ipall) | ih(iphex) | id(ipdec) | ib(ipbin)}\n", msg);
	exit(1);
}

unsigned long
ipencode(const char *ip)
{
	unsigned int octets[4];
	int n, i;
	unsigned long ret;

	n = sscanf(ip, "%u.%u.%u.%u", &octets[0], &octets[1], &octets[2], &octets[3]);
	if (n < 1) {
		return 0;
	}
	
	/* Combine octets into a single decimal value */
	ret = (unsigned long)octets[n-1];
	for (i = 0; i < n-1; i++) {
		ret |= (unsigned long)octets[i] << (24 - i*8);
	}

	return ret;
}

int
main(int argc, char *argv[])
{
	int ch, i;
	char *format;
	char ip[16];
	unsigned long ip_enc;

	if (argc != 2) die(argv[0]);
	format = argv[1];

	if (format[0] == 'i') {
		if (fgets(ip, sizeof(ip), stdin) == NULL) {
			return 1;
		}
		ip_enc = ipencode(ip);
		if (!ip_enc) return 1;

		if (!strcmp(format, "ih"))
			printf("0x%lx\n", ip_enc);
		else if (!strcmp(format, "id"))
			printf("%lu\n", ip_enc);
		else if (!strcmp(format, "ib")) {
			for (i = 31; i >= 0; i--)
				putchar ((ip_enc >> i) & 1 ? '1':'0');
			putchar('\n');
		} else if (!strcmp(format, "ia"))
			printf("hex: 0x%lx\ndec: %lu\n", ip_enc, ip_enc);
		else
			die(argv[0]);
	} else {
		while ((ch = getchar()) != EOF) {
			if (!strcmp(format,"h"))
				printf("%%%02X", ch);
			else if (!strcmp(format, "d"))
				printf("%d,", ch);
			else if (!strcmp(format, "gbk"))
				printf("%%E5%%98%%%X%X",
				       (((ch >> 4) & 0xF)+8) & 0xF,
				       ch & 0xF);
			else
				die(argv[0]);
		}
	}

	return 0;
}

