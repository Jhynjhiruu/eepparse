#include "shiftjis.h"

#include <string.h>
#include <malloc.h>
#include <sys/types.h>

const char * shiftjis[64] = {
    " ", "｡", "｢", "｣", "､", "･", "ｦ", "ｧ", "ｨ", "ｩ", "ｪ", "ｫ", "ｬ", "ｭ", "ｮ", "ｯ",
    "ｰ", "ｱ", "ｲ", "ｳ", "ｴ", "ｵ", "ｶ", "ｷ", "ｸ", "ｹ", "ｺ", "ｻ", "ｼ", "ｽ", "ｾ", "ｿ",
    "ﾀ", "ﾁ", "ﾂ", "ﾃ", "ﾄ", "ﾅ", "ﾆ", "ﾇ", "ﾈ", "ﾉ", "ﾊ", "ﾋ", "ﾌ", "ﾍ", "ﾎ", "ﾏ",
    "ﾐ", "ﾑ", "ﾒ", "ﾓ", "ﾔ", "ﾕ", "ﾖ", "ﾗ", "ﾘ", "ﾙ", "ﾚ", "ﾛ", "ﾜ", "ﾝ", "ﾞ", "ﾟ",
};

size_t read_shiftjis_string(char ** dest, const char * str, off_t len)
{
    const char * strptr = str;
    const char * readptr;
    char * copyptr;
    
    size_t strsize = 0;
    char * buf;
    unsigned char c;
    
    while ((c = *strptr++) != '\0' && strptr - str <= len)
    {
        if (c < 0x80)
        {
            strsize++;
        }
        else
        {
            strsize += strlen(shiftjis[c - 0xA0]);
        }
    }
    
    buf = malloc(strsize + 1);
    if (buf == NULL)
    {
        *dest = NULL;
        return 0;
    }
    
    copyptr = buf;
    strptr = str;
    
    while ((c = *strptr++) != '\0' && strptr - str <= len)
    {
        if (c < 0x80)
        {
            if (c == '/')   c = '_';
            *copyptr++ = c;
        }
        else
        {
            readptr = shiftjis[c - 0xA0];
            while ((c = *readptr++) != '\0')
            {
                *copyptr++ = c;
            }
        }
    }
    
    *copyptr = '\0';
    
    *dest = buf;
    
    return strsize;
}
