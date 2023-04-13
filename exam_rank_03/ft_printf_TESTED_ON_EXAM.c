#include <unistd.h>
#include <stdarg.h>

static int ft_strlen(char *str)
{
    int count;

    if (!str)
        return (0);
    count = 0;
    while (str[count] != '\0')
        count++;
    return (count);
}

static int ft_putchar(int c)
{
    write(1, &c, 1);
    return (1);
}

static int ft_putstr(char *str)
{
    int len;
    
    if (!str)
    {
        write(1, "(null)", 6);
        return (6);
    }
    len = ft_strlen(str);
    write(1, str, len);
    return (len);
}

static int ft_count_digits(int nbr)
{
    int count;

    count = 0;
    if (nbr == -2147483648)
        return (10);
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

static int  ft_putnbr(int nbr)
{
    int count;

	if (nbr == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
    count = 0;
	if (nbr < 0)
	{
		ft_putchar('-');
		nbr *= -1;
        count++;
	}
    count += ft_count_digits(nbr);
	if (nbr < 10)
		ft_putchar(nbr + '0');
	else if (nbr > 9)
	{
		ft_putnbr(nbr / 10);
		ft_putnbr(nbr % 10);
	}
    return (count);
}

static int ft_puthexa(unsigned int nbr)
{
    int count;
    int pos;
    char    hexa[12];

    count = 0;
    pos = 0;
    if (nbr == 0)
    {
        ft_putchar('0');
        return (1);
    }
    while  (nbr > 0)
    {
        hexa[count] = "0123456789abcdef"[nbr % 16];
        nbr /= 16;
        count++;
    }
    pos = count - 1;
    while (pos >= 0)
    {
        ft_putchar(hexa[pos]);
        pos--;
    }
    return (count);
}

static int ft_print_format(char format, va_list args)
{
    int count;

    count = 0;
    if (format == 's')
        count += ft_putstr(va_arg(args, char *));
    if (format == 'd')
		count += ft_putnbr(va_arg(args, int));
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