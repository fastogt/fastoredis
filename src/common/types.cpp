#include "common/types.h"

#include "common/convert2string.h"

namespace common
{
    std::string escapedText(const std::string &str)
    {
        if(!str.empty() && str[str.length()-1] != '\n'){
            return str + "\n";
        }
        return str;
    }

    std::string doubleEscapedText(std::string str)
    {
        if(!str.empty()){
            if(str[str.length()-1] != '\n'){
                str += "\n";
            }

            if(str[0] != '\n'){
                str = "\n" + str;
            }
        }
        return str;
    }
}
