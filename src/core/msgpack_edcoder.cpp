#include "core/msgpack_edcoder.h"

#include "core/lua_engine.h"

namespace fastoredis
{
    MsgPackEDcoder::MsgPackEDcoder()
        : IEDcoder(MsgPack)
    {

    }

    common::ErrorValueSPtr MsgPackEDcoder::encodeImpl(const std::string& data, std::string& out)
    {        
        return LuaEngine::instance().mpPack(data, out);
    }

    common::ErrorValueSPtr MsgPackEDcoder::decodeImpl(const std::string& data, std::string& out)
    {
        return LuaEngine::instance().mpUnPack(data, out);
    }
}
