#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

size_t	ft_strlen_x(char *str, char limit)
{
	size_t	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len] && str[len] != limit)
		len++;
	return (len);
}

int	ft_find_newline(char *stash)
{
	size_t	count;

	if (!stash)
		return (0);
	count = 0;
	while (stash[count])
	{
		if (stash[count] == '\n')
			return (1);
		count++;
	}
	return (0);
}

char	*ft_strcpy_x(char *dest, char *src, char limit)
{
	size_t	count;

	count = 0;
	while (src[count] && src[count] != limit)
	{
		dest[count] = src[count];
		count++;
	}
	if (limit == '\n')
	{
		dest[count] = '\n';
		dest[count + 1] = '\0';
	}
	else
		dest[count] = '\0';
	return (dest);
}

char *ft_fill_stash(char *stash, char *buffer)
{
	size_t	size;
	char	*new_stash;

	size = ft_strlen_x(stash, '\0');
	if (size == 0)
	{
		new_stash = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
		if (!new_stash)
			return (NULL);
		ft_strcpy_x(new_stash, buffer, '\0');
	}
	else
	{
		new_stash = (char *)malloc((size + BUFFER_SIZE + 1) * sizeof(char));
		if (!new_stash)
			return (NULL);
		ft_strcpy_x(new_stash, stash, '\0');
		ft_strcpy_x(new_stash + size, buffer, '\0');
		free(stash);
	}
	return (new_stash);
}

char	*ft_clean_stash(char *stash, size_t line_size)
{
	char	*new_stash;
	size_t	size;

	size = ft_strlen_x(stash, '\0');
	if (size == line_size)
	{
		free(stash);
		return (NULL);
	}
	new_stash = (char *)malloc((size - line_size + 1) * sizeof(char));
	if (!new_stash)
		return (NULL);
	ft_strcpy_x(new_stash, stash + line_size, '\0');
	free(stash);
	stash = NULL;
	return (new_stash);
}

char	*ft_getline(char *stash)
{
	char	*line;
	size_t	size;

	size = ft_strlen_x(stash, '\n');
	if (ft_find_newline(stash))
	{
		line = (char *)malloc((size + 2) * sizeof(char));
		if (!line)
			return (NULL);
		ft_strcpy_x(line, stash, '\n');
	}
	else
	{
		line = (char *)malloc((size + 1) * sizeof(char));
		if (!line)
			return (NULL);
		ft_strcpy_x(line, stash, '\0');
	}
	return (line);
}

char	*ft_fill_with_zero(char *buffer)
{
	size_t	count;

	count = 0;
	while (count <= BUFFER_SIZE)
	{
		buffer[count] = '\0';
		count++;
	}
	return (buffer);
}

char *get_next_line(int fd)
{
	static char	*stash;
	int	reading;
	char	*line;
	char	buffer[BUFFER_SIZE + 1];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	ft_fill_with_zero(buffer);
	while ((reading = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		stash = ft_fill_stash(stash, buffer);
		ft_fill_with_zero(buffer);
		if (ft_find_newline(stash))
			break;
	}
	if (reading == 0 && !stash)
		return (NULL);
	line = ft_getline(stash);
	stash = ft_clean_stash(stash, ft_strlen_x(line, '\0'));
	return (line);
}
