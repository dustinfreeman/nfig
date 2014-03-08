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
            clr.r = (int)a[0].get<long>();
            clr.g = (int)a[1].get<long>();
            clr.b = (int)a[2].get<long>();
        
            _chunk.add_parameter_by_tag(value_name, clr);
        }
    }
};

template <>
Colour nfig::get_default_value<Colour>() {
    return Colour();
}

int main() {
    testfig config;

	if(!config.load_file("../../config.json.txt")) {
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
    
    std::cout << "All tests finished! \n";
    return 0;
}


