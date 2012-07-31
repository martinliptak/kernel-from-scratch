int atoi(const char *str)
{
	int n = 0;
	int neg = 0;
	int base = 10;

	if (*str == '-')
	{
		neg = 1;
		++str;
	}
	
	if (*str == '0')
	{
		++str;
		if (*str == 'x')
		{
			base = 16;
			++str;
		}
		else
			base = 8;
	}
	
	while (*str)
	{
		if (!isdigit(*str))
		{
			if (base > 10 && tolower(*str) >= 'a' 
					&& tolower(*str) <= 'a' + base - 11)
			{
				n *= base;
				n += *str - 'a' + 10;
			}
			else
				break;
		}
		else
		{
			n *= base;
			n += *str - '0';
		}
		++str;
	}
	if (neg)
		n = -n;
	return n;
}

int atoin(const char *str, int *pn)
{
	int neg = 0;
	int i, n;
	int base = 10;
	int count = 0;

	if (*str == '-')
	{
		neg = 1;
		++str;
		++count;
	}

	if (*str == '0')
	{
		++str;
		++count;
		if (*str == 'x')
		{
			base = 16;
			++str;
			++count;
		}
		else
			base = 8;
	}

	for (n = 0, i = 0; str[i]; ++i)
	{
		if (!isdigit(str[i]))
		{
			if (base > 10 && tolower(str[i]) >= 'a' 
					&& tolower(str[i]) <= 'a' + base - 11)
			{
				n *= base;
				n += str[i] - 'a' + 10;
			}
			else
				break;
		}
		else
		{
			n *= base;
			n += str[i] - '0';
		}
	}
	count += i;
	if (neg)
		n = -n;
	*pn = n;
	return count;
}

