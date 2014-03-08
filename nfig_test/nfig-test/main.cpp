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

	if(!nfig::load_file("../../config.json.txt")) {
		std::cerr << "Could not load file. \n";
		return -1;
	} 
	
	assert(nfig::get<bool>("MUTE_MUSIC") == false);
	assert(nfig::get<int>("WIDTH") == 100);
	assert(nfig::get<int>("HEIGHT") == 100);
	assert(nfig::get<float>("WEIGHT") == 10.3);
	assert(strcmp(nfig::get<const char*>("MUTE_MUSIC"), "This is a sweet title") == 0);

}


