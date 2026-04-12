#pragma once
#include <cstring>
#include <utility>
#include <iostream>
#include "types.hpp"
namespace bedrock {
    class string : public withlength_t {
        private:
            char* data = nullptr;
            usize capacity = 0;

        public:

            string(const char* buffer) : data(new char[strlen(buffer)+1]{'\0'}), capacity(strlen(buffer)+1) {
                strcpy(data, buffer);
            }

            string(const char* buffer, usize maxlength) : data(new char[maxlength+1]{'\0'}), capacity(maxlength+1) {
                usize i = 0;
                bool use_maxlen = true;

                if (maxlength > strlen(buffer))
                    strcpy(data, buffer);
                else {
                    strncpy(data, buffer, maxlength);
                    if (data[i] != '\0')
                        data[i] = '\0';
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

            const char* cstr() const {
                return data;
            }

            char& operator[](usize i) {
                return data[i];
            }

            const char& operator[](usize i) const {
                return data[i];
            }

            string substr(usize index, usize lenght) const {
                return string{data+index, lenght};
            }

            usize size() const noexcept { return strlen(data); }

            void append(const string& s) {
                usize data_lenght = strlen(data);
                usize sdata_lenght = strlen(s.data);
                
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
                data[0] = 0;
            }

            char* begin() { return data; }
            char* end() { return data+size(); }



            ~string() {
                delete[] data;
                capacity = 0;
            }
    };
    
    
}

bedrock::string operator+(const char* p, const bedrock::string s) {
    bedrock::string out = p;
    out += s;
    return out;
}

std::ostream& operator<<(std::ostream& out, const bedrock::string& s) {
    out << s.cstr();
    return out;
}