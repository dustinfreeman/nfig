nfig
====

A header-only C++ config library with a focus on ease of spontaneous use over performance. There is no direct access to variables, but instead each variable is accessed like:

    int width = nfig.get<int>("I_WIDTH")

The way I'm using nfig is that I NEVER, EVER create corresponding variables to config value, as with the "width" variable above. This choice is made to reduce the overhead of creating new config values. Trying to code spontaneously when I need to create new config values in different locations in code (globals extern, globals local, parsing config, config file itself) is awful.

Config values are created in a JSON-formatted file, for example:

	{
		"B_MUTE_MUSIC": false,
		"I_WIDTH": 100,
		"I_HEIGHT": 100,
		"F_WEIGHT": 10.3,
		"S_TITLE": "This is a sweet title"
	}
	
where the prefix before the underline must correspond to each type. This allows for creating custom config value types in extensions to nfig, such as SFML's sf::Color

	{
		"CLR_BACKGROUND": [255, 128, 0]
	}

Test code is included.

## Dependencies

For handling parameters of unknown type:

[riffer](https://github.com/dustinfreeman/riffer)

For parsing JSON-format config files:

[picojson](https://github.com/ptarabbia/picojson)
