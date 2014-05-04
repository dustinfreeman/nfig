//
//  main.cpp
//  nfig-test
//
//  Created by Dustin Freeman on 08-03-14.
//  Copyright (c) 2014 dustinfreeman. All rights reserved.
//

#include <iostream>
#include <assert.h>

#include <nfig.h>

#define NFIG_FILENAME "../../config.json.txt"

struct Colour {
    int r,g,b;
    inline bool operator==(const Colour& other)
    {return r == other.r && g == other.g && b == other.b;}
};

struct testfig : nfig {
protected:
    virtual void load_value(std::string value_name, pj::value value) {
        nfig::load_value(value_name, value);
        
        if (value_name.substr(0, 4) == "CLR_") {
            pj::array& a = value.get<pj::array>();
            Colour clr;
            clr.r = (int)a[0].get<double>();
            clr.g = (int)a[1].get<double>();
            clr.b = (int)a[2].get<double>();
        
            _chunk.add_parameter_by_tag(value_name, clr);
        }
    }

	virtual pj::value create_value(std::string value_name) {
		if (value_name.substr(0, 4) == "CLR_") {
			Colour *clr = _chunk.get_parameter_by_tag<Colour>(value_name);

			pj::array a;
			a.push_back(pj::value((double)clr->r));
			a.push_back(pj::value((double)clr->g));
			a.push_back(pj::value((double)clr->b));

			return pj::value(a);
		} else {
			return nfig::create_value(value_name);
		}
	}

};

template <>
Colour nfig::get_default_value<Colour>() {
    return Colour();
}

int test_get() {
	testfig config;

	if(!config.load_file(NFIG_FILENAME)) {
		std::cerr << "Could not load file. \n";
		return -1;
	} 
	
	assert(config.get<bool>("B_MUTE_MUSIC") == false);
	assert(config.get<int>("I_WIDTH") == 100);
	assert(config.get<int>("I_HEIGHT") == 100);
    assert(config.get<float>("F_WEIGHT") == 10.3f);
	assert(config.get<std::string>("S_TITLE") == "This is a sweet title");
    
    Colour test_colour;
    test_colour.r = 255;
    test_colour.g = 128;
    test_colour.b = 0;
    Colour got_colour = config.get<Colour>("CLR_BACKGROUND");
    
    assert(test_colour == got_colour);

	return 0;
}

int test_set() {
	testfig config;

	if(!config.load_file(NFIG_FILENAME)) {
		std::cerr << "Could not load file. \n";
		return -1;
	}

	const bool TEST_MUSIC = true;
	const int TEST_WIDTH = 120;
	const float TEST_WEIGHT = 5.2f;
	const std::string TEST_STRING = "This is a sweet test title string";

	config.set("B_MUTE_MUSIC", TEST_MUSIC);
	config.set("I_WIDTH", TEST_WIDTH);
	config.set("F_WEIGHT", TEST_WEIGHT);
	config.set("S_TITLE", TEST_STRING);

	assert(config.get<bool>("B_MUTE_MUSIC") == TEST_MUSIC);
	assert(config.get<int>("I_WIDTH") == TEST_WIDTH);
	assert(config.get<float>("F_WEIGHT") == TEST_WEIGHT);
	assert(config.get<std::string>("S_TITLE") == TEST_STRING);

	return 0;
}

int test_write_out() {
	//copy config file for testing
	const std::string NFIG_FILENAME_WRITE_OUT = "../../config_writeout.json.txt";
	CopyFile(NFIG_FILENAME, NFIG_FILENAME_WRITE_OUT);
	std::cout << "Copied config file to " << NFIG_FILENAME_WRITE_OUT << " for testing.\n";

	//load config and change value
	testfig config;
	if(!config.load_file(NFIG_FILENAME_WRITE_OUT)) {
		std::cerr << "Could not load file. \n";
		return -1;
	}
	const std::string TEST_WRITEOUT_STRING = "This is a sweet write-out test";
	config.set("S_TITLE", TEST_WRITEOUT_STRING);

	//write out
	config.write_out();

	//read in and test.
	testfig readconfig;
	if(!readconfig.load_file(NFIG_FILENAME_WRITE_OUT)) {
		std::cerr << "Could not load file. \n";
		return -1;
	}

	assert(readconfig.get<std::string>("S_TITLE") == TEST_WRITEOUT_STRING);

	return 0;
}

int main() {
    test_get();
	test_set();
	test_write_out();
    
    std::cout << "All tests finished! \n";
    return 0;
}


