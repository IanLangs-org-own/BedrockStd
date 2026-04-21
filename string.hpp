#pragma once
#include <cstring>
#include <cstdint>
#include <utility>
#include <iostream>
#include "types.hpp"
namespace bedrock {
    namespace types {
        class string {
            private:
                char* data = nullptr;
                ::bedrock::types::usize capacity = 0;

            public:

                string(const char* buffer) : data(new char[strlen(buffer)+1]{'\0'}), capacity(strlen(buffer)+1) {
                    strcpy(data, buffer);
                }

                string(const char* buffer, ::bedrock::types::usize maxlength) : data(new char[maxlength+1]{'\0'}), capacity(maxlength+1) {

                    if (maxlength > strlen(buffer))
                        strcpy(data, buffer);
                    else {
                        strncpy(data, buffer, maxlength);
                        if (data[maxlength] != '\0')
                            data[maxlength] = '\0';
                    }

                    
                }

                string() : data(new char[1]{'\0'}), capacity(1){}
                string(const string& other) : data(new char[other.capacity]{'\0'}), capacity(other.capacity) {
                    strcpy(data, other.data);
                }

                string(string&& other) {
                    std::swap(other.data, this->data);
                    std::swap(other.capacity, this->capacity);
                }

                string(long double other) {
                    char buffer[64];
                    snprintf(buffer, sizeof(buffer), "%Lf", other);
                    data = new char[strlen(buffer)+1]{'\0'};
                    capacity = strlen(buffer)+1;
                    strcpy(data, buffer);
                }

                string(long long other) {
                    char buffer[64];
                    snprintf(buffer, sizeof(buffer), "%lld", other);
                    data = new char[strlen(buffer)+1]{'\0'};
                    capacity = strlen(buffer)+1;
                    strcpy(data, buffer);
                }

                const char* cstr() const {
                    return data;
                }

                char& operator[](::bedrock::types::usize i) {
                    return data[i];
                }

                const char& operator[](::bedrock::types::usize i) const {
                    return data[i];
                }

                string substr(::bedrock::types::usize index, ::bedrock::types::usize lenght) const {
                    return string{data+index, lenght};
                }

                ::bedrock::types::usize size() const noexcept { return strlen(data); }

                void append(const string& s) {
                    ::bedrock::types::usize data_lenght = strlen(data);
                    ::bedrock::types::usize sdata_lenght = strlen(s.data);
                    
                    char* new_data;
                    if (capacity <= (sdata_lenght + data_lenght +1)) {
                        new_data = new char[capacity+ sdata_lenght +1]{'\0'};
                        capacity += sdata_lenght +1;
                    } else
                        new_data = new char[capacity]{'\0'};

                    strcat(new_data, data);
                    strcat(new_data, s.data);
                    delete[] data;
                    data = new_data;
                }

                void append(const char* s) {
                    char* new_data;
                    if (capacity <= (strlen(s) + strlen(data) +1)) {
                        new_data = new char[capacity+ strlen(s) +1]{'\0'};
                        capacity += strlen(s) +1;
                    } else
                        new_data = new char[capacity]{'\0'};

                    strcat(new_data, data);
                    strcat(new_data, s);
                    delete[] data;
                    data = new_data;
                }

                void append(const char c) {
                    char* new_data;
                    usize len = size();
                    if (capacity <= (strlen(data) + 2)) {
                        new_data = new char[capacity+ 2]{'\0'};
                        capacity += 2;
                    } else
                        new_data = new char[capacity]{'\0'};

                    strcat(new_data, data);
                    new_data[len] = c;
                    new_data[len+1] = 0;
                    delete[] data;
                    data = new_data;
                }

                string operator+(const string s) const {
                    string cpy(*this);
                    cpy += s;
                    return cpy;
                }

                string operator+(const char* s) const {
                    string cpy(*this);
                    cpy += s;
                    return cpy;
                }

                string operator+(const char c) const {
                    string cpy(*this) ;
                    cpy += c;
                    return cpy;
                }

                string& operator+=(const string& s) {
                    append(s);
                    return *this;
                }
                
                string& operator+=(const char* s) {
                    append(s);
                    return *this;
                }

                string& operator+=(const char c) {
                    append(c);
                    return *this;
                }

                string& operator=(const string& s) {
                    set(s);
                    return *this;
                }
                
                string& operator=(const char* s) {
                    set(s);
                    return *this;
                }

                string& operator=(const char c) {
                    set(c);
                    return *this;
                }

                void set(const string& s) {
                    clear();
                    append(s);
                }

                void set(const char* s) {
                    clear();
                    append(s);
                }

                void set(const char& s) {
                    clear();
                    append(s);
                }

                void clear() {
                    delete[] data;
                    data = new char[1]{'\0'};
                    capacity = 1;
                }

                char* begin() { return data; }
                char* end() { return data+size(); }



                ~string() {
                    delete[] data;
                    capacity = 0;
                }
        };
    }
    
}

bedrock::types::string operator+(const char* p, const bedrock::types::string s) {
    bedrock::types::string out = p;
    out += s;
    return out;
}

std::ostream& operator<<(std::ostream& out, const bedrock::types::string& s) {
    out << s.cstr();
    return out;
}