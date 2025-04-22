#include <functional>

#include "oidhasher.h"


using namespace std;
using namespace SignCert;

size_t ByteVectorHash::operator()(const vector<uint8_t> &v) const
{
    size_t hash_i = 0;

    for (uint8_t b : v)
    {
        hash_i ^= hash<uint8_t>{}(b) + 0x9e3779b9 + (hash_i << 6) + (hash_i >> 2);
    }

    return hash_i;
}

bool ByteVectorEqual::operator()(const vector<uint8_t> &a, const vector<uint8_t> &b) const
{
    return a == b;
}
