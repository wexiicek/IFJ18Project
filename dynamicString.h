#ifndef _DYNAMIC_STRING_H
#define _DYNAMIC_STRING_H


#include <stdbool.h>


/**
 * @struct Representation of dynamic string.
 */
typedef struct
{
	char *str; /// string ened by '\0' byte
	unsigned int length; /// lenght of string
	unsigned int allocSize; /// number of chars alocated for string
} DynamicString;


/**
 * Inicialization of dynamic string.
 *
 * @param s Pointer to dynamic string.
 * @return True if inicialization was successful, false otherwise.
 */
bool dynamicstringinit(DynamicString *s);

/**
 * Frees alocated memory for dynamic string.
 *
 * @param s Pointer to dynamic string.
 */
void dynamicStringFree(DynamicString *s);

/**
 * Clear dynamic string content.
 *
 * @param s Pointer to dynamic string.
 */
void dynamicStringClear(DynamicString *s);

/**
 * Add char to the end of dynamic string.
 *
 * @param s Dynamic string.
 * @param c Char to add.
 * @return True if it was successful, false otherwise.
 */
bool dynamicStringAddChar(DynamicString *s, char c);

/**
 * Add constant string to the end of dynamic string.
 *
 * @param s Dynamic string.
 * @param const_string Constant string.
 * @return True if it was successful, false otherwise.
 */
bool dynamicStringAddConstStr(DynamicString *s, const char *constString);

/**
 * Compare dynamic string with constant string.
 *
 * @param dynamic_string Dynamic string.
 * @param const_string Constant string.
 * @return Returns 1, 0, or -1, according as the s1 is greater than, equal to, or less than the s2.
 */
int dynamicStringCmpConstStr(DynamicString *dynamicString, const char *constString);

/**
 * Copy src string to dest string.
 *
 * @param src Source string.
 * @param dest Destination string.
 * @return True if it was successful, false otherwise.
 */
bool dynamicStringCopy(DynamicString *src, DynamicString *dest);


#endif // _DYNAMIC_STRING_H