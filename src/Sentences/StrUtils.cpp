/*
 * StrUtils.cpp
 *
 * (C) Copyright 2016 Pavel Bobov.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <string.h>
#include <math.h>

#include "StrUtils.h"

const char* nextToken(const char* p) {
  return strchr(p, ',') + 1;
}

void addComma(char *str) {
  strcat(str, ",");
}

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.

 */
char* ltoa2(long value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

float scl10(float f, int scale) {
  if (scale <= 0) return f;
  if (scale == 1) return f * 10.0; 
  if (scale == 2) return f * 100.0; 
  if (scale == 3) return f * 1000.0; 
  if (scale == 4) return f * 10000.0; 
  return scale <= 0 ? f : 10.0 * scl10(f, --scale);
}

char* ftoa(float f, char str[], int precision) {
  double w;
  double r = modf(f, &w);

  ltoa2((long)w, str, 10);
  strcat(str, ".");

  char* p = strchr(str, '\0');
  
  char buff[10];
  ltoa2((long)scl10(r, precision), buff, 10);
  zeropad(buff, precision);
  strcat(str, buff);

  size_t len = strlen(p);
  memset(p + len, '0', precision - len);
  p[precision] = '\0';

  return str;
}

char* zeropad(char str[], size_t width) {
  size_t len = strlen(str);
  size_t shift = width - len;

  if (shift > 0) {
    memmove(str + shift, str, ++len);
    memset(str, '0', shift);
  }

  return str;
}

