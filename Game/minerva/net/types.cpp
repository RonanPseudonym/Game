#include "../include.h"

using namespace Minerva;

uint64_t Minerva::Net::DumpFloat(long double f, unsigned bits, unsigned expbits)
{
    long double fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (f == 0.0) return 0; // get this special case out of the way

    // check sign and begin normalization
    if (f < 0) { sign = 1; fnorm = -f; }
    else { sign = 0; fnorm = f; }

    // get the normalized form of f and track the exponent
    shift = 0;
    while (fnorm >= 2.0) { fnorm /= 2.0; shift++; }
    while (fnorm < 1.0) { fnorm *= 2.0; shift--; }
    fnorm = fnorm - 1.0;

    // calculate the binary form (non-float) of the significand data
    significand = fnorm * ((1LL << significandbits) + 0.5f);

    // get the biased exponent
    exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias

    // return the final answer
    return (sign << (bits - 1)) | (exp << (bits - expbits - 1)) | significand;
}

long double Net::LoadFloat(uint64_t i, unsigned bits, unsigned expbits)
{
    long double result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (i == 0) return 0.0;

    // pull the significand
    result = (i & ((1LL << significandbits) - 1)); // mask
    result /= (1LL << significandbits); // convert back to float
    result += 1.0f; // add the one back on

    // deal with the exponent
    bias = (1 << (expbits - 1)) - 1;
    shift = ((i >> significandbits) & ((1LL << expbits) - 1)) - bias;
    while (shift > 0) { result *= 2.0; shift--; }
    while (shift < 0) { result /= 2.0; shift++; }

    // sign it
    result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;

    return result;
}

    Net::Int2::Int2() {

    }

    Net::Int2::Int2(int16_t n) {
        internal = n;
    }

    void Net::Int2::Load(unsigned char* data_stream, unsigned int* index) {
        unsigned int i2 = ((unsigned int)data_stream[*index] << 8) | data_stream[*index + 1];

        // change unsigned numbers to signed
        if (i2 <= 0x7fffu) { internal = i2; }
        else { internal = -1 - (unsigned int)(0xffffu - i2); }

        *index += 2;
    }

    void Net::Int2::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 'i';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::Int2::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = internal >> 8;
        data_stream[*index + 1] = internal;

        *index += 2;
    }

    unsigned int Net::Int2::Size() {
        return 2;
    }

    std::string Net::Int2::ToString() {
        return std::to_string(internal);
    }

    Net::IntU2::IntU2() {

    }

    Net::IntU2::IntU2(uint16_t n) {
        internal = n;
    }

    void Net::IntU2::Load(unsigned char* data_stream, unsigned int* index) {
        internal = ((unsigned int)data_stream[*index] << 8) | data_stream[*index + 1];

        *index += 2;
    }

    void Net::IntU2::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 'I';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::IntU2::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = internal >> 8;
        data_stream[*index + 1] = internal;

        *index += 2;
    }

    unsigned int Net::IntU2::Size() {
        return 2;
    }

    std::string Net::IntU2::ToString() {
        return std::to_string(internal);
    }

    Net::Int4::Int4() {

    }

    Net::Int4::Int4(int32_t n) {
        internal = n;
    }

    void Net::Int4::Load(unsigned char* data_stream, unsigned int* index) {
        unsigned long int i2 = ((unsigned long int)data_stream[*index] << 24) |
            ((unsigned long int)data_stream[*index + 1] << 16) |
            ((unsigned long int)data_stream[*index + 2] << 8) |
            data_stream[*index + 3];

        // change unsigned numbers to signed
        if (i2 <= 0x7fffffffu) { internal = i2; }
        else { internal = -1 - (long int)(0xffffffffu - i2); }

        *index += 4;
    }

    void Net::Int4::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 'j';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::Int4::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = internal >> 24;
        data_stream[*index + 1] = internal >> 16;
        data_stream[*index + 2] = internal >> 8;
        data_stream[*index + 3] = internal;

        *index += 4;
    }

    unsigned int Net::Int4::Size() {
        return 4;
    }

    std::string Net::Int4::ToString() {
        return std::to_string(internal);
    }

    Net::IntU4::IntU4() {

    }

    Net::IntU4::IntU4(uint32_t n) {
        internal = n;
    }

    void Net::IntU4::Load(unsigned char* data_stream, unsigned int* index) {
        internal = ((unsigned long int)data_stream[*index] << 24) |
            ((unsigned long int)data_stream[*index + 1] << 16) |
            ((unsigned long int)data_stream[*index + 2] << 8) |
            data_stream[*index + 3];

        *index += 4;
    }

    void Net::IntU4::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 'J';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::IntU4::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = internal >> 24;
        data_stream[*index + 1] = internal >> 16;
        data_stream[*index + 2] = internal >> 8;
        data_stream[*index + 3] = internal;

        *index += 4;
    }

    unsigned int Net::IntU4::Size() {
        return 4;
    }

    std::string Net::IntU4::ToString() {
        return std::to_string(internal);
    }

    Net::Int8::Int8() {

    }

    Net::Int8::Int8(int64_t n) {
        internal = n;
    }

    void Net::Int8::Load(unsigned char* data_stream, unsigned int* index) {
        unsigned long long int i2 = ((unsigned long long int)data_stream[*index + 0] << 56) |
            ((unsigned long long int)data_stream[*index + 1] << 48) |
            ((unsigned long long int)data_stream[*index + 2] << 40) |
            ((unsigned long long int)data_stream[*index + 3] << 32) |
            ((unsigned long long int)data_stream[*index + 4] << 24) |
            ((unsigned long long int)data_stream[*index + 5] << 16) |
            ((unsigned long long int)data_stream[*index + 6] << 8) |
            data_stream[*index + 7];
        long long int i;

        // change unsigned numbers to signed
        if (i2 <= 0x7fffffffffffffffu) { internal = i2; }
        else { internal = -1 - (long long int)(0xffffffffffffffffu - i2); }

        *index += 8;
    }

    void Net::Int8::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 'k';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::Int8::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index + 0] = internal >> 56;
        data_stream[*index + 1] = internal >> 48;
        data_stream[*index + 2] = internal >> 40;
        data_stream[*index + 3] = internal >> 32;
        data_stream[*index + 4] = internal >> 24;
        data_stream[*index + 5] = internal >> 16;
        data_stream[*index + 6] = internal >> 8;
        data_stream[*index + 7] = internal;

        *index += 8;
    }

    unsigned int Net::Int8::Size() {
        return 8;
    }

    std::string Net::Int8::ToString() {
        return std::to_string(internal);
    }

    Net::IntU8::IntU8() {

    }

    Net::IntU8::IntU8(uint64_t n) {
        internal = n;
    }

    void Net::IntU8::Load(unsigned char* data_stream, unsigned int* index) {
        internal =
            ((unsigned long long int)data_stream[*index + 0] << 56) |
            ((unsigned long long int)data_stream[*index + 1] << 48) |
            ((unsigned long long int)data_stream[*index + 2] << 40) |
            ((unsigned long long int)data_stream[*index + 3] << 32) |
            ((unsigned long long int)data_stream[*index + 4] << 24) |
            ((unsigned long long int)data_stream[*index + 5] << 16) |
            ((unsigned long long int)data_stream[*index + 6] << 8) |
            data_stream[*index + 7];

        *index += 8;
    }

    void Net::IntU8::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 'K';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::IntU8::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index + 0] = internal >> 56;
        data_stream[*index + 1] = internal >> 48;
        data_stream[*index + 2] = internal >> 40;
        data_stream[*index + 3] = internal >> 32;
        data_stream[*index + 4] = internal >> 24;
        data_stream[*index + 5] = internal >> 16;
        data_stream[*index + 6] = internal >> 8;
        data_stream[*index + 7] = internal;

        *index += 8;
    }

    unsigned int Net::IntU8::Size() {
        return 8;
    }

    std::string Net::IntU8::ToString() {
        return std::to_string(internal);
    }

    Net::Float::Float() {

    }

    Net::Float::Float(float f) {
        internal = f;
    }

    void Net::Float::Load(unsigned char* data_stream, unsigned int* index) {
        uint32_t data = ((unsigned long int)data_stream[*index + 0] << 24) |
            ((unsigned long int)data_stream[*index + 1] << 16) |
            ((unsigned long int)data_stream[*index + 2] << 8) |
            data_stream[*index + 3];

        internal = LoadFloat(data, 32, 8);

        *index += 4;
    }

    void Net::Float::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 'f';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::Float::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        uint32_t i = DumpFloat(internal, 32, 8);

        data_stream[*index + 0] = i >> 24;
        data_stream[*index + 1] = i >> 16;
        data_stream[*index + 2] = i >> 8;
        data_stream[*index + 3] = i;

        *index += 4;
    }

    unsigned int Net::Float::Size() {
        return 4;
    }

    std::string Net::Float::ToString() {
        return std::to_string(internal);
    }

    Net::Double::Double() {

    }

    Net::Double::Double(double f) {
        internal = f;
    }

    void Net::Double::Load(unsigned char* data_stream, unsigned int* index) {
        uint64_t data =
            ((unsigned long long int)data_stream[*index + 0] << 56) |
            ((unsigned long long int)data_stream[*index + 1] << 48) |
            ((unsigned long long int)data_stream[*index + 2] << 40) |
            ((unsigned long long int)data_stream[*index + 3] << 32) |
            ((unsigned long long int)data_stream[*index + 4] << 24) |
            ((unsigned long long int)data_stream[*index + 5] << 16) |
            ((unsigned long long int)data_stream[*index + 6] << 8) |
            data_stream[*index + 7];

        internal = LoadFloat(data, 64, 11);

        *index += 8;
    }

    void Net::Double::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 'F';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::Double::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        uint64_t i = DumpFloat(internal, 64, 11);

        data_stream[*index + 0] = i >> 56;
        data_stream[*index + 1] = i >> 48;
        data_stream[*index + 2] = i >> 40;
        data_stream[*index + 3] = i >> 32;
        data_stream[*index + 4] = i >> 24;
        data_stream[*index + 5] = i >> 16;
        data_stream[*index + 6] = i >> 8;
        data_stream[*index + 7] = i;

        *index += 8;
    }

    unsigned int Net::Double::Size() {
        return 8;
    }

    std::string Net::Double::ToString() {
        return std::to_string(internal);
    }

    Net::String::String() {

    }

    Net::String::String(char* str) {
        internal = str;
        size.internal = strlen(str);
    }

    Net::String::String(unsigned int _size) {
        size.internal = _size;
    }

    void Net::String::Load(unsigned char* data_stream, unsigned int* index) {
        size.Load(data_stream, index);
        internal = (char*)malloc(size.internal);
        memcpy(internal, &(data_stream[*index]), size.internal);
        *index += size.internal;
    }

    void Net::String::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 's';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::String::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        size.DumpRaw(data_stream, index);
        memcpy(&data_stream[*index], internal, size.internal);
        *index += size.internal;
    }

    unsigned int Net::String::Size() {
        return size.internal + 2;
    }

    std::string Net::String::ToString() {
        return std::string(internal, size.internal);
    }

    Net::Any::Any() {

    }

    Net::Any::Any(Value* _internal) {
        internal = _internal;
    }

    void Net::Any::Load(unsigned char* data_stream, unsigned int* index) {
        internal = LoadValueType(data_stream, index);
        (*index)++;

        internal->Load(data_stream, index);
    }
    void Net::Any::Dump(unsigned char* data_stream, unsigned int* index) {
        internal->Dump(data_stream, index);
    }
    unsigned int Net::Any::Size() { // without identifier
        return internal->Size();
    }
    unsigned int Net::Any::NetSize() {
        return internal->Size() + 1;
    }
    std::string Net::Any::ToString() {
        return internal->ToString();
    }

    Net::ComponentValue::ComponentValue() {
        id = IntU4();
        key = String();
        value = Any();
    }
    Net::ComponentValue::ComponentValue(IntU4 _id, String _key, Any _value) {
        id = _id;
        key = _key;
        value = _value;
    }

    void Net::ComponentValue::Load(unsigned char* data_stream, unsigned int* index) {
        id.Load(data_stream, index);
        key.Load(data_stream, index);
        value.Load(data_stream, index);
    }

    void Net::ComponentValue::Dump(unsigned char* data_stream, unsigned int* index) {
        data_stream[*index] = 'c';
        (*index)++;

        DumpRaw(data_stream, index);
    }

    void Net::ComponentValue::DumpRaw(unsigned char* data_stream, unsigned int* index) {
        id.DumpRaw(data_stream, index);
        key.DumpRaw(data_stream, index);
        value.Dump(data_stream, index); // needs id because type is unknown
    }

    unsigned int Net::ComponentValue::Size() {
        return id.Size() + key.Size() + value.Size();
    }

    std::string Net::ComponentValue::ToString() {
        return id.ToString() + ":" + key.ToString() + " = " + value.ToString();
    }

Net::Value* Net::LoadValueType(unsigned char* data_stream, unsigned int* index) {
    switch (data_stream[*index]) {
    case 'i': return new Int2();
    case 'I': return new IntU2();
    case 'j': return new Int4();
    case 'J': return new IntU4();
    case 'k': return new Int8();
    case 'K': return new IntU8();
    case 'f': return new Float();
    case 'F': return new Double();
    case 's': return new String();
    case 'c': return new ComponentValue();
    }
}

    Net::Packet::Packet() {

    }

    Net::Packet::Packet(PacketType _type) {
        type = _type;
    }

    Net::Packet::Packet(PacketType _type, std::vector<Any> _data) {
        type = _type;
        data = _data;
    }

    void Net::Packet::operator+=(Any value) {
        data.push_back(value);
    }

    unsigned int Net::Packet::Size() {
        unsigned int size = 3; // header size

        for (Any i : data) {
            size += i.NetSize(); // netsize means +1 (char encoding)
        }

        return size;
    }

    unsigned char* Net::Packet::Dump() {
        unsigned char* out = (unsigned char*)malloc(Size());

        out[0] = (unsigned char)type;

        unsigned int* index = new unsigned int(1);
        IntU2((unsigned int)data.size()).DumpRaw(out, index);


        // index now is 3

        for (Any i : data) {
            i.Dump(out, index);
        }

        return out;
    }

    void Net::Packet::Load(unsigned char* in) {
        type = (PacketType)in[0];

        unsigned int* i = new unsigned int(1);
        IntU2 count;
        count.Load(in, i);

        for (int j = 0; j < count.internal; j++) {
            Any val;
            val.Load(in, i);
            data.push_back(val);
        }

        free(in);
    }

    std::string Net::Packet::ToString() {
        std::string out = "packet " + std::to_string(type) + " [" + std::to_string(Size()) + " bytes] {\n";
        for (auto val : data) {
            out += "  " + val.ToString() + "\n";
        }
        out += "}";

        return out;
    }