/* bitstream.hpp */
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define INITSIZE     10
#define INCREMENT    10


class BitStream {
    friend std::ostream& operator<<(std::ostream& out, const BitStream& bs);
    
    public:
        explicit BitStream();
        ~BitStream();
        bool operator[](int n) const;
        void set(const int n, const bool bit);
        void append_str(const char *str);
        void append_bitstr(const char *bstr);
        void append_bit(const bool bit);
        
    private:
        unsigned char *data;
        int len;
        int size;
};

BitStream::BitStream() {
    data = (unsigned char *)std::malloc(sizeof(unsigned char) * INITSIZE);
    if (!data) throw std::runtime_error("Malloc overflow.");
    len = 0;
    size = INITSIZE;
}

BitStream::~BitStream() {
	free(data);
	data = NULL;
	len = 0;
	size = 0;
}

bool BitStream::operator[](int pos) const {
    if (pos < 0 || pos > len)
        throw std::invalid_argument( "parameter overflow" );
    return data[pos / 8] & (1 << (7 - (pos % 8))) ? 1 : 0;
}

void BitStream::set(const int pos, const bool bit) {
    if (pos < 0 || pos > len)
        throw std::invalid_argument( "parameter overflow" );
    if(bit)
        data[pos / 8] = data[pos / 8] | (1 << (7 - (pos % 8)));
    else
        data[pos / 8] = data[pos / 8] & ( ~(1 << (7 - (pos % 8))));
}

void BitStream::append_str(const char *str){
    int l = 0;
    while (str[l]){
        if (len / 8 + l >= size){
            unsigned char *p = (unsigned char *)std::realloc(data, sizeof(unsigned char) * (size + INCREMENT));
            if (!p) throw std::runtime_error("Malloc overflow.");
            data = p;
            size += INCREMENT;
        }
        data[l] = (unsigned char)str[l];
        l++;
    }
    len += l * 8;
}

void BitStream::append_bitstr(const char *bstr){
    while (*bstr == '0' || *bstr == '1' || *bstr == ' ')
        if (*bstr != ' ') 
            append_bit(*bstr++ - '0');
        else
            bstr++;
}

void BitStream::append_bit(const bool bit){
    if (len + 1 >= size * 8){
        unsigned char *p = (unsigned char *)std::realloc(data, sizeof(unsigned char)*(size + INCREMENT));
        if(!p) throw std::runtime_error("Malloc overflow.");
        data = p;
        size += INCREMENT;
    }
    set(len++, bit);
}


std::ostream& operator<<(std::ostream& out, const BitStream& bs) {
    int i;
    out << " Stream Length:\n  " << bs.len << "\n Content binary bits: \n  ";
    for(i = 0; i < bs.len; i++)
        out << bs[i] << ((i + 1) % 8 ? "" : ((i + 1) % 64 ? " " : "\n  "));
    return out;
}

