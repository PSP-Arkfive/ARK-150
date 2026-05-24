/*
 * This file is part of PRO CFW.

 * PRO CFW is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO CFW is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO CFW. If not, see <http://www.gnu.org/licenses/ .
 */

#include <pspsdk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int tolower(int s)
{
    if((s >= 'A') && (s <= 'Z'))
        s = 'a' + (s - 'A');

    return s;
}

void lowerString(char* orig, char* ret, int strSize){
    int i=0;
    while (*(orig+i) && i<strSize-1){
        *(ret+i) = *(orig+i);
        if (*(orig+i) >= 'A' && *(orig+i) <= 'Z')
            *(ret+i) += 0x20;
        i++;
    }
    *(ret+i) = 0;
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    unsigned char c1, c2;

    if (p1 == p2 || n == 0)
        return 0;
    
    if (s1 == NULL || s2 == NULL){
        return (int)s1 - (int)s2;
    }

    do {
        c1 = tolower(*p1);
        c2 = tolower(*p2);

        if (--n == 0 || c1 == '\0')
        	break;

        ++p1;
        ++p2;
    } while (c1 == c2);

    return c1 - c2;
}

int strcasecmp(const char *s1, const char *s2)
{
    return strncasecmp(s1, s2, (size_t)-1);
}
