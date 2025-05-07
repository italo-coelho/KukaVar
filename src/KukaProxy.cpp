#include "KukaProxy.h"
#include "KukaVar.h"

KukaProxy::KukaProxy(const String& key, KukaVar* parent)
{
    _key = key;
    _parent = parent;
}


KukaProxy::operator int() const
{
    String val = _parent->read(_key);
    if(val.startsWith("ERROR->"))
        return KUKA_INT_ERROR;
    else
        return val.toInt();
}

KukaProxy::operator float() const
{
    String val = _parent->read(_key);
    if(val.startsWith("ERROR->"))
        return KUKA_FLOAT_ERROR;
    else
        return val.toFloat();
}

KukaProxy::operator double() const
{
    String val = _parent->read(_key);
    if(val.startsWith("ERROR->"))
        return KUKA_DOUBLE_ERROR;
    else
        return val.toDouble();
}

KukaProxy::operator bool() const
{
    String val = _parent->read(_key);
    return val == "TRUE";
}

KukaProxy::operator String() const
{
    String val = _parent->read(_key);
    if(val.startsWith("ERROR->"))
        return KUKA_STRING_ERROR;
    else
        return val;
}


KukaProxy& KukaProxy::operator=(int value) 
{
    _parent->write(_key, String(value));
    return *this;
}

KukaProxy& KukaProxy::operator=(float value) 
{
    _parent->write(_key, String(value));
    return *this;
}

KukaProxy& KukaProxy::operator=(double value) 
{
    _parent->write(_key, String(value));
    return *this;
}

KukaProxy& KukaProxy::operator=(bool value) 
{
    _parent->write(_key, value ? "TRUE" : "FALSE");
    return *this;
}

KukaProxy& KukaProxy::operator=(const String& value) 
{
    _parent->write(_key, value);
    return *this;
}

KukaProxy& KukaProxy::operator=(const char* value) 
{
    _parent->write(_key, String(value));
    return *this;
}


KukaProxy KukaProxy::operator[](int index) const 
{
    return KukaProxy(_key + "[" + index + "]", _parent);
}