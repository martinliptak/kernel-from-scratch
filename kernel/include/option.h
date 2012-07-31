#ifndef _OPTION_H
#define _OPTION_H

#define MAX_ARG_LEN 32

enum option_type
{
	OPT_TYPE_BOOL, 
	OPT_TYPE_STRING, 
	OPT_TYPE_SIGNED
};

struct option
{
	const char		*name;
	enum option_type	type;
	void			*value;
};

#endif
