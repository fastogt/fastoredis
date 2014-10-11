#include "common/types.h"

#include <string.h>

namespace common
{
    string16 escapedText(const string16 &str)
    {
        if(!str.empty() && str[str.length()-1] != '\n'){
            return str + UTEXT("\n");
        }
        return str;
    }
}
