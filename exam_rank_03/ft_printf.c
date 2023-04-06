#include <unistd.h>
#include <stdlib.h>
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

int ft_putnbr(int nbr)
{
    int count;
    int size;
    char    *number;
    int sign;

    if (nbr == 0)
    {
        ft_putchar('0');
        return (1);
    }
    if (nbr == -2147483648)
    {
        ft_putstr("-2147483648");
        return (11);
    }
    sign = 0;
    if (nbr < 0)
    {
        nbr *= -1;
        sign = 1;
    }
    size = ft_count_digits(nbr);
    count = 0;
    number = (char *)malloc((size + 1) * sizeof(char));
    if (!number)
        return (0);
    while (count < size)
    {
        number[count] = nbr % 10 + '0';
        nbr /= 10;
        count++;
    }
    count = 0;
    if (sign == 1)
    {
        ft_putchar('-');
        count++;
    }
    size--;
    while (size >= 0)
    {
        ft_putchar(number[size]);
        size--;
        count++;
    }
    free(number);
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


/* int main(void)
{
    int r;
    char *s = NULL;

    r = 0;
    int count = 0;
    r += F("%d: Simple test\n", count++); // 0
    r += F("%d: ", count++); // 1
    r += F("%d: --Format---", count++); // 2
    r += F("%d: \n", count++); // 3
    r += F("%d: %d\n",count++,  0); // 4
    r += F("%d: %d\n",count++,  42); // 5
    r += F("%d: %d\n",count++,  1); // 6
    r += F("%d: %d\n",count++,  5454); // 7
    r += F("%d: %d\n",count++,  (int)2147483647); // 8
    r += F("%d: %d\n",count++,  (int)2147483648); // 9
    r += F("%d: %d\n",count++,  (int)-2147483648); // 10
    r += F("%d: %d\n",count++,  (int)-2147483649); // 11
    r += F("%d: \n", count++); // 12
    r += F("%d: %x\n",count++,  0); // 13
    r += F("%d: %x\n",count++,  42); // 14
    r += F("%d: %x\n",count++,  1); // 15
    r += F("%d: %x\n",count++,  5454); // 16
    r += F("%d: %x\n",count++,  (int)2147483647); // 17
    r += F("%d: %x\n",count++,  (int)2147483648); // 18
    r += F("%d: %x\n",count++,  (int)-2147483648); // 19
    r += F("%d: %x\n",count++,  (int)-2147483649); // 20
    r += F("%d: %x\n",count++,  (int)0xFFFFFFFF); // 21
    r += F("%d: \n", count++); // 22
    r += F("%d: %s\n",count++,  ""); // 23
    r += F("%d: %s\n",count++,  "toto"); // 24
    r += F("%d: %s\n",count++,  "wiurwuyrhwrywuier"); // 25
    r += F("%d: %s\n",count++,  NULL); // 26
    r += F("%d: -%s-%s-%s-%s-\n", count++ , "", "toto", "wiurwuyrhwrywuier", NULL); // 27
    r += F("%d: \n--Mixed---\n", count++); // 28
    r += F("%d: %d%x%d%x%d%x%d%x\n",count++,  0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648); // 29
    r += F("%d: -%d-%x-%d-%x-%d-%x-%d-%x-\n",count++,  0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648); // 30
    r += F("%d: \n", count++ ); // 31
    r += F("%d: %s%s%s%s\n",count++,  "", "toto", "wiurwuyrhwrywuier", NULL); // 32
    r += F("%d: -%s-%s-%s-%s-\n",count++,  "", "toto", "wiurwuyrhwrywuier", NULL); // 33
    printf("written: %d\n", r);
    printf("======================================\n");
    printf("======================================\n");
    printf("======================================\n");
    count = 0;
    r = 0;
    r += printf("%d: Simple test\n", count++); // 0
    r += printf("%d: ", count++); // 1
    r += printf("%d: --Format---", count++); // 2
    r += printf("%d: \n", count++); // 3
    r += printf("%d: %d\n",count++,  0); // 4
    r += printf("%d: %d\n",count++,  42); // 5
    r += printf("%d: %d\n",count++,  1); // 6
    r += printf("%d: %d\n",count++,  5454); // 7
    r += printf("%d: %d\n",count++,  (int)2147483647); // 8
    r += printf("%d: %d\n",count++,  (int)2147483648); // 9
    r += printf("%d: %d\n",count++,  (int)-2147483648); // 10
    r += printf("%d: %d\n",count++,  (int)-2147483649); // 11
    r += printf("%d: \n", count++); // 12
    r += printf("%d: %x\n",count++,  0); // 13
    r += printf("%d: %x\n",count++,  42); // 14
    r += printf("%d: %x\n",count++,  1); // 15
    r += printf("%d: %x\n",count++,  5454); // 16
    r += printf("%d: %x\n",count++,  (int)2147483647); // 17
    r += printf("%d: %x\n",count++,  (int)2147483648); // 18
    r += printf("%d: %x\n",count++,  (int)-2147483648); // 19
    r += printf("%d: %x\n",count++,  (int)-2147483649); // 20
    r += printf("%d: %x\n",count++,  (int)0xFFFFFFFF); // 21
    r += printf("%d: \n", count++); // 22
    r += printf("%d: %s\n",count++,  ""); // 23
    r += printf("%d: %s\n",count++,  "toto"); // 24
    r += printf("%d: %s\n",count++,  "wiurwuyrhwrywuier"); // 25
    r += printf("%d: %s\n",count++,  s); // 26
    r += printf("%d: -%s-%s-%s-%s-\n", count++ , "", "toto", "wiurwuyrhwrywuier", s); // 27
    r += printf("%d: \n--Mixed---\n", count++); // 28
    r += printf("%d: %d%x%d%x%d%x%d%x\n",count++,  0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648); // 29
    r += printf("%d: -%d-%x-%d-%x-%d-%x-%d-%x-\n",count++,  0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648); // 30
    r += printf("%d: \n", count++ ); // 31
    r += printf("%d: %s%s%s%s\n",count++,  "", "toto", "wiurwuyrhwrywuier", s); // 32
    r += printf("%d: -%s-%s-%s-%s-\n",count++,  "", "toto", "wiurwuyrhwrywuier", s); // 33
    printf("written: %d\n", r);
    return (0);
} */
