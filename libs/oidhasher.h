#ifndef OIDHASHER_H
#define OIDHASHER_H

#include <cstdint>
#include <vector>


using namespace std;

namespace SignCert {

struct ByteVectorHash
{
    size_t operator()(const vector<uint8_t> &v) const;
};

struct ByteVectorEqual
{
    bool operator()(const vector<uint8_t> &a, const vector<uint8_t> &b) const;
};

} // namespace SignCert

#endif // OIDHASHER_H
