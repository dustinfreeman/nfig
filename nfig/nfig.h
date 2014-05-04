#include <map>
#include <fstream>
#include <string>

#include <riffer.h>

#include <picojson.h>
#define pj picojson

#define _DEFAULT_BOOL true
#define _DEFAULT_INT -1
#define _DEFAULT_FLOAT -0.5f
#define _DEFAULT_CHAR_STR "default string"
#define _DEFAULT_STR "default std::string"

class nfig {
protected:
    //=================
    rfr::Chunk _chunk;
    
    virtual void load_value(std::string value_name, pj::value value) {
        // this function should be overloaded for custom types.
        if (value_name.substr(0, 2) == "B_")
            _chunk.add_parameter_by_tag(value_name, value.get<bool>());
        else if (value_name.substr(0, 2) == "I_")
            _chunk.add_parameter_by_tag(value_name, (int)value.get<long>());
        else if (value_name.substr(0, 2) == "S_")
            _chunk.add_parameter_by_tag(value_name, value.get<std::string>());
        else if (value_name.substr(0, 2) == "F_") {// || value_name.find("_") == std::string::npos)
            float _value;
            //error catching in case someone forgets to add ".0" at the end of a number to force it to float.
            if (value.is<int>())
                _value = (float)value.get<long>();
            else
                _value = (float)value.get<double>();
            _chunk.add_parameter_by_tag(value_name, _value);
            
        }
    }

public:

    template<class T>
    T get_default_value();
    
    bool load_file(const char* filename) {
        std::ifstream file;
        file.open (filename);
        if (!file.is_open()) {
            std::cerr << "Error " << filename << " file not open.\n";
            return false;
        }
        
        std::string err;
        picojson::value root;
        err = pj::parse(root, file);
        
        if (! err.empty()) {
            std::cerr << " file parse error: " << err << std::endl;
        } else {
            //std::cout << "No parse error!\n";
        }

        pj::value::object obj = root.get<pj::object>();
        for (pj::value::object::const_iterator i = obj.begin();
            i != obj.end();
            ++i) {
            //std::cout << i->first << "\n";
            
            load_value(i->first, i->second);
        }
        return true;
	}
    
    bool load_file(std::string filename) {
        return load_file(filename.c_str());
    }
    
    template<class T>
	T get (std::string value_name){
        T* value = _chunk.get_parameter_by_tag<T>(value_name);
        
        if (value == nullptr) {
            std::cerr << "Warning: nfig could not find value " << value_name << ". Returning default value.\n";
            return get_default_value<T>();
        } else {
            return *value;
        }
    }

};

template<>
inline bool nfig::get_default_value<bool>() {
    return _DEFAULT_BOOL;
}

template<>
inline int nfig::get_default_value<int>() {
    return _DEFAULT_INT;
}

template<>
inline float nfig::get_default_value<float>() {
    return _DEFAULT_FLOAT;
}

template<>
inline const char* nfig::get_default_value<const char*>() {
    return _DEFAULT_CHAR_STR;
}

template<>
inline std::string nfig::get_default_value<std::string>() {
    return _DEFAULT_STR;
}
