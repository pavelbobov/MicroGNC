/*
 * StrUtils.h
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

#ifndef _STRUTILS_H_
#define _STRUTILS_H_

#include <stdlib.h>

/*
 * Returns pointer to the next character after ',' in the specified string
 * 
 * @param p string
 * @return next character after ',' in the specified string
 */
const char* nextToken(const char* p);

/*
 * Adds ',' to the end of the specified string
 * 
 * @param str string
 */
void  addComma(char *str);

/*
 * Converts floating point number to decimal string representation
 * 
 * @param f number
 * @param str string buffer
 * @param precision number of digits after '.'
 * @return decimal string representation
 */
char* ftoa(float f, char str[], int precision);

/*
 * Adds zeros to the beginning of the specified string until the total string 
 * length is equal to width.
 * 
 * @param str string
 * @param width desired string length
 * @return padded string
 */
char* zeropad(char str[], size_t width);

#endif /* _STRUTILS_H_ */
