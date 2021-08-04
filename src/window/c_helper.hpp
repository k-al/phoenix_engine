#include <exception>
#include <cstring>
#include <string>
#include <vector>

#include <iostream>

struct CstringArr {
    char** array;
    uint32_t length = 0;
    
    ~CstringArr () {
        this->clear();
    }
    
    bool set_array (std::vector<std::string> input) {
        
        // dont overwrite dierectly, because memory must be freed
        if (length != 0) {
            this->clear();
        }
        
        this->array = new char*[input.size() + 1];
        this->array[input.size()] = nullptr;
        
        // use this->length as iterator to have always the number of allocated list elements saved
        for (/*length is 0*/; this->length < input.size(); ++this->length) {
            char* temp;
            try {
                temp = new char[input[this->length].size() + 1];
            } catch (std::bad_alloc e) {
                throw(e);
            }
            strncpy(temp, input[this->length].c_str(), input[this->length].size());
            temp[input[this->length].size()] = '\0';
            this->array[this->length] = temp;
        }
        
        // return if all the required allocations are sucessfull
        return this->length == input.size();
    }
    
    void clear () {
        // free all the allocated things
        if (this->length > 0) {
            for (uint32_t i = 0; i < this->length; ++i) {
                delete[] array[i];
            }
            delete[] array;
            
            this->length = 0;
        }
    }
    
    // release the ownership of the array
    // caller must take care of the freeing
    char** release () {
        this->length = 0;
        return this->array;
    }
};
