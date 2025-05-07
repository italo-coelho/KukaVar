#ifndef KUKA_PROXY_H
#define KUKA_PROXY_H

#include <limits>
#include <Arduino.h>

const int KUKA_INT_ERROR = std::numeric_limits<int>::min();
const float KUKA_FLOAT_ERROR = std::numeric_limits<float>::min();
const double KUKA_DOUBLE_ERROR = std::numeric_limits<double>::min();
const String KUKA_STRING_ERROR = "__KUKA_ERROR__";

class KukaVar;

class KukaProxy 
{
    private:
        String _key;
        KukaVar* _parent;
    
    public:
        KukaProxy(const String& key, KukaVar* parent);

        // Read operators
        operator int() const;
        operator float() const;
        operator double() const;
        operator bool() const;
        operator String() const;

        // Write operators
        KukaProxy& operator=(int value);
        KukaProxy& operator=(float value);
        KukaProxy& operator=(double value);
        KukaProxy& operator=(bool value);
        KukaProxy& operator=(const String& value);
        KukaProxy& operator=(const char* value);

        // Array-style access
        KukaProxy operator[](int index) const;
};

#endif //!KUKA_PROXY_H