#include <map>

#include <riffer.h>

#include <picojson.h>

namespace nfig {
    //DEFAULTS=========
    //a default is returned whenever the get() value doesn't exist.
    
    template<class T>
    T get_default_value();
    
    const bool _DEFAULT_BOOL = true;
    const int _DEFAULT_INT = -1;
    const float _DEFAULT_FLOAT = -0.5f;
    const char* _DEFAULT_CHAR_STR = "default string";
    
    template<>
    bool get_default_value<bool>() {
        return _DEFAULT_BOOL;
    }
    
    template<>
    int get_default_value<int>() {
        return _DEFAULT_INT;
    }
    
    template<>
    float get_default_value<float>() {
        return _DEFAULT_FLOAT;
    }
    
    template<>
    const char* get_default_value<const char*>() {
        return _DEFAULT_CHAR_STR;
    }
    
    //=================
    
    rfr::Chunk _nfig_chunk;
    
	bool load_file(const char* filename) {
        
    
    
        //TODO load _nfig_chunk

        return false;
	}
    
    template<class T>
	T get (std::string value_name){
        T* value = _nfig_chunk.get_parameter_by_tag<T>(value_name);
        
        if (value == nullptr) {
            std::cerr << "Warning: nfig could not find value " << value_name << ". Returning default value.\n";
            return get_default_value<T>();
        } else {
            return *value;
        }
    }

};

//template<>
//bool nfig::get<bool>(std::string value_name){
//    if (get_value<bool>(value_name, &nfig_value)) {
//        nfig_value->value;
//    } else {
//        return nfig::_DEFAULT_BOOL;
//    }
//}
//
//template<>
//int nfig::get<int>(std::string value_name){
//	return nfig::_DEFAULT_INT;
//}
//
//template<>
//float nfig::get<float>(std::string value_name){
//	return nfig::_DEFAULT_FLOAT;
//}
//
//template<>
//const char* nfig::get<const char*>(std::string value_name){
//	return nfig::_DEFAULT_CHAR_STR;
//}
