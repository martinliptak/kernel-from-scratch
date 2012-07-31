#include "option.h"
#include "library.h"

int opt_verbosity = 3;

struct option options[] = {
	{"verbosity", OPT_TYPE_SIGNED, &opt_verbosity}, 
	{NULL, 0, NULL}
};

static void parse_option(const char *opt, const char *arg)
{
	int i;
	int sn;
	
	for (i = 0; options[i].name; ++i)
	{
		if (strcmp(options[i].name, opt) == 0)
		{
			switch (options[i].type)
			{
			case OPT_TYPE_BOOL:
				if (arg)
				{
					if (strcmp(arg, "no") == 0 || strcmp(arg, "0") == 0)
						*(bool *)options[i].value = false;
					else if (strcmp(arg, "yes") == 0 
							|| strcmp(arg, "1") == 0)
						*(bool *)options[i].value = true;
					else
						panic("Option \'%s\' requires a boolean "
								"argument (yes | no, 1 | 0)", 
								opt);
				}
				else
					*(bool *)options[i].value = true;
				break;
			case OPT_TYPE_STRING:
				if (!arg)
					panic("Option \'%s\' requires a string argument", 
							opt);
				strcpy((char *)options[i].value, arg);
				break;
			case OPT_TYPE_SIGNED:
				if (!arg || sscanf(arg, "%i", (int *)options[i].value) == 0)
					panic("Option \'%s\' requires a signed numurical "
							"argument", opt);
				break;
			}
			return;
		}
	}
	panic("Bad option name \'%s\'", opt);
}

static void option_found(char *opt)
{
	int		i;
	const char 	*arg;

	for (i = 0, arg = NULL; opt[i]; ++i)
		if (opt[i] == '=')
		{
			arg = opt + i + 1;
			opt[i] = 0;
			break;
		}

	parse_option(opt, arg);
}

void parse_cmdline(const char *cmdline)
{
	const char 	*c;
	dstring_t 	*opt = dstring_new(16);
	
	printk("<1>Command line: %s\n", cmdline);
	/* Skip the 1st option (path to the kernel image). */
	for (c = cmdline; *c != ' ' && *c != 0; ++c)
		;
	if (*c == 0)
	{
		dstring_free(opt);
		return;
	}
	
	/* Skip spaces. */
	while (*c == ' ') 
		++c;
	
	for (; *c; ++c)
	{
		if (isblank(*c) && !isblank(c[1]))
		{
			dstring_append_char(opt, 0);
			option_found(opt->str);
			/* Delete the contents of "opt". */
			dstring_free(opt);
			opt = dstring_new(16);
			/* Skip spaces. */
			while (*c == ' ')
				++c;
		}
		else
			dstring_append_char(opt, *c);
	}
	dstring_append_char(opt, *c);
	option_found(opt->str);
	dstring_free(opt);
}

