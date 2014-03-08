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

int main() {

    nfig config;

	if(!config.load_file("../../config.json.txt")) {
		std::cerr << "Could not load file. \n";
		return -1;
	} 
	
	assert(config.get<bool>("MUTE_MUSIC") == false);
	assert(config.get<int>("WIDTH") == 100);
	assert(config.get<int>("HEIGHT") == 100);
	assert(config.get<float>("WEIGHT") == 10.3);
	assert(strcmp(config.get<const char*>("MUTE_MUSIC"), "This is a sweet title") == 0);

}


