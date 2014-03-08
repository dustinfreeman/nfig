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

struct colour {
    int r,g,b;
};

int main() {
    nfig config;

	if(!config.load_file("../../config.json.txt")) {
		std::cerr << "Could not load file. \n";
		return -1;
	} 
	
	assert(config.get<bool>("B_MUTE_MUSIC") == false);
	assert(config.get<int>("I_WIDTH") == 100);
	assert(config.get<int>("I_HEIGHT") == 100);
    assert(config.get<float>("F_WEIGHT") == 10.3f);
	assert(config.get<std::string>("S_TITLE") == "This is a sweet title");
    
    std::cout << "All tests finished! \n";
    return 0;
}


