#include <unistd.h>
#include <stdarg.h>

int ft_strlen(char *str)
{
    int count;

    if (!str)
        return (0);
    count = 0;
    while (str[count] != '\0')
        count++;
    return (count);
}

int ft_putchar(char c)
{
    write(1, &c, 1);
    return (1);
}

int ft_putstr(char *str)
{
    int len;
    
    if (!str)
        return (ft_putstr("(null)"));
    len = ft_strlen(str);
    write(1, str, len);
    return (len);
}

int ft_count_digits(int nbr)
{
    int count;

    count = 0;
    if (nbr == -2147483648)
        return (11);
    if (nbr == 0)
        return (1);
    if (nbr < 0)
        nbr *= -1;
    while (nbr > 0)
    {
        nbr /= 10;
        count ++;
    }
    return (count);
}

void	ft_putnbr(int nbr)
{
	if (nbr == -2147483648)
	{
		write(1, "-2147483648", 11);
		return ;
	}
	else if (nbr < 0)
	{
		nbr *= -1;
		ft_putchar('-');
		ft_putnbr(nbr);
	}
	else if (nbr < 10)
		ft_putchar(nbr + 48);
	else if (nbr > 9)
	{
		ft_putnbr(nbr / 10);
		ft_putnbr(nbr % 10);
	}
}

int	ft_print_nbr(int nbr)
{
	int	count;

	count = ft_count_digits(nbr);
	ft_putnbr(nbr);
	return (count);
}

int ft_puthexa(unsigned int nbr)
{
    int count;
    int pos;
    char    hexa[12];

    count = 0;
    pos = 0;
    if (nbr == 0)
    {
        ft_putchar('0');
        count++;
    }
    while  (nbr != 0)
    {
        hexa[pos] = "0123456789abcdef"[nbr % 16];
        nbr /= 16;
        count++;
        pos++;
    }
    pos--;
    while (pos >= 0)
    {
        ft_putchar(hexa[pos]);
        pos--;
    }
    return (count);
}

int ft_print_format(char format, va_list args)
{
    int count;

    count = 0;
    if (format == 's')
        count += ft_putstr(va_arg(args, char *));
    if (format == 'd')
		count += ft_print_nbr(va_arg(args, int));
    else if (format == 'x')
        count += ft_puthexa(va_arg(args, unsigned int));
    return (count);
}

int ft_printf(const char *format, ... )
{
    int count;
    int pos;
    va_list args;

    count = 0;
    pos = 0;
    if (!format)
        return (count);
    va_start(args, format);
    while (format[pos])
    {
        if (format[pos] == '%')
        {
            count += ft_print_format(format[pos + 1], args);
            pos += 2;
        }
        else
        {
            ft_putchar(format[pos]);
            pos++;
            count++;
        }
    }
    va_end(args);
    return (count);
}