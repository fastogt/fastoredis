#include "common/types.h"

#include "common/convert2string.h"

namespace common
{
    string16 escapedText(const string16 &str)
    {
        if(!str.empty() && str[str.length()-1] != '\n'){
            return str + convertToString16("\n");
        }
        return str;
    }
}
