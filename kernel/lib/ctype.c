int isdigit(char c)
{
	return ('0' <= c) && (c <= '9');
}

int isblank(char c)
{
	return c == ' ' || c == '\t';
}

int tolower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 32;
	else
		return c;
}

int toupper(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - 32;
	else
		return c;
}

