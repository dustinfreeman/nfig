#include <map>
#include <fstream>
#include <string>

#include <riffer.h>

#include <picojson.h>
#define pj picojson

#define NFIG_DEFAULT_BOOL true
#define NFIG_DEFAULT_INT -1
#define NFIG_DEFAULT_FLOAT -0.5f
#define NFIG_DEFAULT_CHAR_STR "default string"
#define NFIG_DEFAULT_STR "default std::string"

#define NFIG_PREFIX_BOOL "B_"
#define NFIG_PREFIX_INT "I_"
#define NFIG_PREFIX_FLOAT "F_"
#define NFIG_PREFIX_STR "S_"

class nfig {
protected:
    //=================
    rfr::Chunk _chunk;
	std::string _filename;
    
    virtual void load_value(std::string value_name, pj::value value) {
        // this function should be overloaded for custom types.
        if (value_name.substr(0, 2) == NFIG_PREFIX_BOOL)
            _chunk.add_parameter_by_tag(value_name, value.get<bool>());
        else if (value_name.substr(0, 2) == NFIG_PREFIX_INT)
            _chunk.add_parameter_by_tag(value_name, (int)value.get<double>());
        else if (value_name.substr(0, 2) == NFIG_PREFIX_STR)
            _chunk.add_parameter_by_tag(value_name, value.get<std::string>());
        else if (value_name.substr(0, 2) == NFIG_PREFIX_FLOAT) {// || value_name.find("_") == std::string::npos)
            float _value;
            //error catching in case someone forgets to add ".0" at the end of a number to force it to float.
            if (value.is<int>())
                _value = (float)value.get<double>();
            else
                _value = (float)value.get<double>();
            _chunk.add_parameter_by_tag(value_name, _value);
            
        }
    }

	virtual pj::value create_value(std::string value_name) {
		pj::value value;
		
		if (value_name.substr(0, 2) == NFIG_PREFIX_BOOL)
            value = pj::value( *(_chunk.get_parameter_by_tag<bool>(value_name)) );
        else if (value_name.substr(0, 2) == NFIG_PREFIX_INT)
            value = pj::value( (double)*(_chunk.get_parameter_by_tag<int>(value_name)) );
        else if (value_name.substr(0, 2) == NFIG_PREFIX_STR)
            value = pj::value( *(_chunk.get_parameter_by_tag<std::string>(value_name)) );
        else if (value_name.substr(0, 2) == NFIG_PREFIX_FLOAT)
			value = pj::value( *(_chunk.get_parameter_by_tag<float>(value_name)) );

		return value;
	}

public:

    template<class T>
    T get_default_value();
    
	bool reload() {
		if (_filename.size() == 0) {
			std::cerr << "Filename not established \n";
			return false;
		}

		std::ifstream file;
        file.open(_filename);
        if (!file.is_open()) {
            std::cerr << "Error " << _filename << " file not open.\n";
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

		file.close();

        return true;
	}

    bool load_file(std::string filename) {
		_filename = filename;

		return reload();
	}

	void write_out() {
		if (_filename.size() == 0) {
			std::cerr << "Filename not established \n";
			return;
		}
		
		pj::value::object root_object;
		//iterate over chunk parameters and write out
		std::map<std::string, std::shared_ptr<rfr::AbstractParam> >::iterator param_it;
		for (param_it = _chunk.params.begin(); param_it != _chunk.params.end(); param_it++) {
			//write_value(param_it->first, param_it->second);
			
			pj::value value = create_value(param_it->first);
			root_object[param_it->first] = pj::value(value);
		}

		pj::value root(root_object);

		std::ofstream file;
		file.open(_filename, std::fstream::out);
		if (!file.is_open()) {
			std::cout << "Output file not open: " << _filename << "\n"; 
		}

		bool prettify = true;
		std::string serialized = root.serialize(prettify);
		file << serialized;

		file.close();
	}
    
    template<class T>
	T get (std::string value_name) {
        T* value = _chunk.get_parameter_by_tag<T>(value_name);
        
        if (value == nullptr) {
            std::cerr << "Warning: nfig could not find value " << value_name << ". Returning default value.\n";
            return get_default_value<T>();
        } else {
            return *value;
        }
    }

	template<class T>
	void set (std::string value_name, T _value);
};

//defaults
template<>
inline bool nfig::get_default_value<bool>() {
    return NFIG_DEFAULT_BOOL;
}
template<>
inline int nfig::get_default_value<int>() {
    return NFIG_DEFAULT_INT;
}
template<>
inline float nfig::get_default_value<float>() {
    return NFIG_DEFAULT_FLOAT;
}
template<>
inline const char* nfig::get_default_value<const char*>() {
    return NFIG_DEFAULT_CHAR_STR;
}
template<>
inline std::string nfig::get_default_value<std::string>() {
    return NFIG_DEFAULT_STR;
}

//sets
//check proper prefix
template<>
void nfig::set(std::string value_name, bool _value) {
	if (value_name.substr(0, 2) != NFIG_PREFIX_BOOL)
		std::cerr << "BOOL value_name did not have prefix " << NFIG_PREFIX_BOOL << "\n";

	_chunk.add_parameter_by_tag(value_name, _value);
}

template<>
void nfig::set(std::string value_name, int _value) {
	if (value_name.substr(0, 2) != NFIG_PREFIX_INT)
		std::cerr << "INT value_name did not have prefix " << NFIG_PREFIX_INT << "\n";

	_chunk.add_parameter_by_tag(value_name, _value);
}

template<>
void nfig::set(std::string value_name, float _value) {
	if (value_name.substr(0, 2) != NFIG_PREFIX_FLOAT)
		std::cerr << "FLOAT value_name did not have prefix " << NFIG_PREFIX_FLOAT << "\n";

	_chunk.add_parameter_by_tag(value_name, _value);
}

template<>
void nfig::set(std::string value_name, std::string _value) {
	if (value_name.substr(0, 2) != NFIG_PREFIX_STR)
		std::cerr << "STRING value_name did not have prefix " << NFIG_PREFIX_STR << "\n";

	_chunk.add_parameter_by_tag(value_name, _value);
}