#include "../../minishell.h"


void	ft_strncpy_2(char *dest, char *src, int size, t_pipex *data)
{
	int i;
	int j;
	int open;


	// CHECK IF OPEN IS NECESSARY !!!
	i = 0;
	j = 0;
	open = data->buf_int;
	// printf("COPY START  SRC: |%s| OPEN : %d\n", src, open);
	while (j < size)
	{
		// printf("COPY LINE 1  SRC: |%s| OPEN : %d\n", src, open);
		// while (((!open && is_quote(src[i]))
		// 	|| (open == 1 && is_quote_one(src[i]))
		// 	|| (open == 2 && is_quote_two(src[i])))
		// 	 && !is_space(src[i - 1]))
		// 	i++;
		if (is_quote_one(src[i]) && !open)
			open = 1;
		else if (is_quote_two(src[i]) && !open)
			open = 2;
		else if ((is_quote_one(src[i]) && open == 1) || (is_quote_two(src[i]) && open == 2))
			open = 0;
		// printf("COPY LINE 2  SRC: |%s| OPEN : %d\n", src, open);
		if (src[i] && ((!is_quote(src[i])) || ((open == 1 && is_quote_two(src[i]))
			|| (open == 2 && is_quote_one(src[i])))))
			// || (open == 2 && is_quote_one(src[i])))) && printf("CPY THIS: %c\n", src[i]))
			dest[j++] = src[i];
		i++;
	}
	data->buf_int = 0;
}

// void	ft_strncpy_3(char *dest, char *src, int size, t_pipex *data)
// {
// 	int i;
// 	int j;
// 	int open;


// 	// CHECK IF OPEN IS NECESSARY !!!
// 	i = 0;
// 	j = 0;
// 	open = data->buf_int;
// 	// printf("COPY START  SRC: |%s| OPEN : %d\n", src, open);
// 	while (j < size)
// 	{
// 		// printf("COPY LINE 1  SRC: |%s| OPEN : %d\n", src, open);
// 		// while (((!open && is_quote(src[i]))
// 		// 	|| (open == 1 && is_quote_one(src[i]))
// 		// 	|| (open == 2 && is_quote_two(src[i])))
// 		// 	 && !is_space(src[i - 1]))
// 		// 	i++;
// 		if (open && i > 0 && is_space(src[i]) && !is_space(src[i - 1]))
// 			data->count_elem++;
// 		if (is_quote_one(src[i]) && !open)
// 			open = 1;
// 		else if (is_quote_two(src[i]) && !open)
// 			open = 2;
// 		else if ((is_quote_one(src[i]) && open == 1) || (is_quote_two(src[i]) && open == 2))
// 			open = 0;
// 		printf("COPY LINE 2  SRC: |%s| OPEN : %d\n", src, open);
// 		if (src[i] && ((!is_quote(src[i])) || ((open == 1 && is_quote_two(src[i]))
// 			|| (open == 2 && is_quote_one(src[i])))))
// 			// || (open == 2 && is_quote_one(src[i])))) && printf("CPY THIS: %c\n", src[i]))
// 			dest[j++] = src[i];
// 		i++;
// 	}
// 	data->buf_int = 0;
// }

int	count_key(char *line, int j, int open)
{
	int k;
	int check;

	k = 0;
	check = 0;
	while (line[j])
	{
		// printf("COUNT KEY POS: %s CHECK: %d\n", line + j, check);
		if (check && open == 2 && (is_space(line[j]) || line[j] == '\n' || is_quote(line[j])))
			break;
		if (check && !open && (!line[j] || is_space(line[j]) || line[j] == '\n'
			|| is_real_pipe(line, j)
			|| is_red_clean(line, j)
			|| line[j] == '/'
			|| line[j] == '\n'			
			|| (line[j] == '$' && check)
			|| is_quote(line[j])))
			break;
		if (check)
			k++;
		// printf("KEY ______ %d\n", k);
		if (line[j] == '$')
			check = 1;
		if (check && !k && line[j + 1] == '?')
			return (1);
		j++;
	}
	return (k);
}

char *extract_key(t_pipex *data, int j, int open, int check)
{
	int k;
	char *key;

	key = malloc(sizeof(char) * (count_key(data->line, j, open) + 1));
	if (!key)
		return (error_code(data), NULL);
	key[count_key(data->line, j, open)] = 0;
	k = 0;
	while (data->line[j])
	{
		// printf("OPEN: %d at %s\n", open, line + j);
		if (check && open == 2 && (is_space(data->line[j]) || data->line[j] == '\n' || is_quote(data->line[j])))
			break;
		if (check && !open && (!data->line[j] || is_space(data->line[j]) || data->line[j] == '\n'
			|| is_real_pipe(data->line, j)
			|| is_red_clean(data->line, j)
			|| data->line[j] == '/'
			|| data->line[j] == '\n'
			|| (data->line[j] == '$' && check)
			|| is_quote(data->line[j])))
			break;
		if (check)
			key[k++] = data->line[j];
		if (data->line[j] == '$')
			check = 1;
		if (check && !k && data->line[j + 1] == '?')
			return (ft_strcpy("?", key), key);
		j++;
	}
	return (key);
}

char *fill_key(t_pipex *data, char *str)
{
	int i;
	char *new;

	new = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new)
		return (error_code(data), NULL);
	new[ft_strlen(str)] = 0;
	i = -1;
	while (str[++i])
		new[i] = str[i];
	return (new);
}

char *find_elem(t_pipex *data, int i, int open)
{
	char	*elem;
	char	*key;
	int		j;

	j = -1;
	key = extract_key(data, i, open, 0);
	// printf("KEY: %s\n", key);
	elem = NULL;
	while (data->cur_env[++j])
	{
		// printf("ELEM: %s | %ld\n", data->cur_env[j], ft_strlen(key));
		if (!ft_strncmp(key, data->cur_env[j], ft_strlen(key)) && data->cur_env[j][ft_strlen(key)] == '=')
			elem = fill_key(data, data->cur_env[j] + (count_key(data->line, i, open) + 1));
	}
	free_str(key);
	key = NULL;
	// printf("FOUND ELEM :%s$\n", elem);
	return (elem);
}

int count_elem_spaces(t_pipex *data, char *elem)
{
	int i;
	int count;

	i = -1;
	count = 0;
	if (!elem)
		return (0);
	while (elem[++i])
	{
		if (i > 0 && is_space(elem[i]) && !is_space(elem[i - 1]))
		{
			data->count_elem++;
			count++;
		}
	}
	return (count);
}

int	count_expansion(t_pipex *data, int i, int open)
{
	int count;
	char *elem;

	count = 0;
	// printf("START COUNT EXPANSION: %d\n", count);
	elem = NULL;
	while (data->line[i])
	{
		// printf("EXPANSION 1: |%s| COUNT: %d OPEN: %d\n", data->line + i, count, open);
		if (open != 1 && data->line[i] == '$' && data->line[i + 1] && data->line[i + 1] != '?')
		{
			elem = find_elem(data, i, open);
			count_elem_spaces(data, elem);
			i += count_chars_2(data, i);
			// printf("ELEM2  : %s\n", elem);
		}
		else if (open != 1 && data->line[i] == '$' && data->line[i + 1] == '?')
		{
			i += 2;
			elem = ft_itoa(data->last_exit_status);
		}
		if (elem)
		{
			count += ft_strlen(elem);
			free(elem);
			elem = NULL;
		}
		// printf("EXPANSION     2: |%s| COUNT: %d OPEN: %d\n", data->line + i, count, open);
		if (is_quote_one(data->line[i]) && !open)
			open = 1;
		else if (is_quote_two(data->line[i]) && !open)
			open = 2;
		else if ((is_quote_one(data->line[i]) && open == 1) || (is_quote_two(data->line[i]) && open == 2))
			open = 0;
		// printf("EXPANSION           3: %s COUNT: %d OPEN: %d\n", data->line + i, count, open);
		if (!open && (!data->line[i] || is_space(data->line[i]) || data->line[i] == '\n'
			|| is_real_pipe(data->line, i)
			|| is_red_clean(data->line, i)
			|| (is_space(data->line[i]) && is_quote(data->line[i + 1]))
			|| ((!data->line[i + 2] || is_space(data->line[i + 2]) || data->line[i + 2] == '\n') && is_quote(data->line[i + 1]))))
			break;
		if (data->line[i] && (((data->line[i] != '$' || (data->line[i] == '$' && is_delim_front(data->line, i + 1))) && !open) || open)
			&& ((!is_quote(data->line[i])) || ((open == 1 && is_quote_two(data->line[i]))
				|| (open == 2 && is_quote_one(data->line[i])))))
			count++;
		if (open || (data->line[i] != '$' || (data->line[i] == '$' && is_delim_front(data->line, i + 1))))
			i++;
	}
	// count += check_for_empty(data, i);
	// printf("COUNT EXPANSION: %d\n", count);
	return (count);
}


char	*expand_it_1(t_pipex *data, int i, int open)
{
	int j;
	char *elem;
	char *new;

	elem = NULL;
	new = malloc(sizeof(char) * (count_expansion(data, i, open) + 1));
	if (!new)
		return (printf("malloc fail\n"), free_str(elem), error_code(data), NULL);
	new[count_expansion(data, i, open)] = 0;
	j = 0;
	while (data->line[i])
	{
		// printf("START EXPANSION:  |%s|     I: %d|       OPEN: %d\n", data->line + i, i ,open);
		if (open != 1 && data->line[i] == '$' && (data->line[i + 1] == '?'))
		{
			elem = ft_itoa(data->last_exit_status);
			data->buf_int = open;
			ft_strncpy_2(new + j, elem, ft_strlen(elem), data);
			i += 2;
		}
		else if (open != 1 && data->line[i] == '$' && data->line[i + 1] != '?')
		{
			elem = find_elem(data, i, open);
			data->buf_int = open;
			ft_strncpy_2(new + j, elem, ft_strlen(elem), data);
			i += count_chars_2(data, i);
		}
		if (elem)
		{
			j += ft_strlen(elem);
			free(elem);
			elem = NULL;
		}
		// printf("LINE: 1	 |%s|   			  I: %d|     OPEN: %d\n", data->line + i, i ,open);
		if (open == 2 && is_quote_two(data->line[i]) && (!data->line[i + 1] || is_space(data->line[i + 1])))
			break;
		if (is_quote_one(data->line[i]) && !open)
			open = 1;
		else if (is_quote_two(data->line[i]) && !open)
			open = 2;
		else if ((is_quote_one(data->line[i]) && open == 1) || (is_quote_two(data->line[i]) && open == 2))
			open = 0;
		// printf("LINE: 	2	 |%s|   			 I: %d|    OPEN: %d\n", data->line + i, i ,open);
		if (!open && (!data->line[i] || is_space(data->line[i]) || data->line[i] == '\n'
			|| is_real_pipe(data->line, i)
			|| is_red_clean(data->line, i)
			|| (is_space(data->line[i]) && is_quote(data->line[i + 1]))
			|| ((!data->line[i + 2] || is_space(data->line[i + 2]) || data->line[i + 2] == '\n') && is_quote(data->line[i + 1]))))
			break;
		if (data->line[i] && (((data->line[i] != '$' || (data->line[i] == '$' && is_delim_front(data->line, i + 1))) && !open) || open)
			&& ((!is_quote(data->line[i])) || ((open == 1 && is_quote_two(data->line[i]))
			|| (open == 2 && is_quote_one(data->line[i])))))
		// if (data->line[i] && (!check_for_empty(data, i) || (!open && check_for_empty(data, i) && is_quote(data->line[i + 1]))) && ((!is_quote(data->line[i])) || ((open == 1 && is_quote_two(data->line[i]))
			// || (open == 2 && is_quote_one(data->line[i]))) || (check_for_empty(data, i) && printf("THIS: %s\n", data->line + i) && is_quote_one(data->line[i + 1]) && is_space(data->line[i]))))
			new[j++] = data->line[i];
		if (open || (data->line[i] != '$' || (data->line[i] == '$' && is_delim_front(data->line, i + 1))))
			i++;
	}
	// printf("NEW: $%s$\n", new);
	return (new);
}

char *malloc_str(size_t size, t_pipex *data)
{
	char *new;

	new = ft_calloc(sizeof(char), (size + 1));
	if (!new)
		return (perror("malloc failed!"), error_code(data), NULL);
	return (new);
}

int	expand_it_2(t_pipex *data, int index_1, int index_2, int index_3)
{
	int i;
	int j;
	int k;
	int count;
	char *buf;

	i = -1;
	j = -1;
	count = -1;
	data->buf_str = data->lines[index_1]->cmnds[index_2][index_3];
	if (!count_elem_spaces(data, data->buf_str))
		return (0);
	// printf("BUF STR: %s\n", data->buf_str);
	while ((data->buf_str[++i] && !is_space(data->buf_str[i]) && ++j != INT_MIN) || data->buf_str[i])
	{
		// printf("STR: %s : J: %d\n", data->buf_str + i, j);
		if ((is_space(data->buf_str[i + 1]) || !data->buf_str[i + 1]) && !is_space(data->buf_str[i]) && ++count != INT_MIN)
		{
			// printf("TRUE\n");
			buf = malloc_str(++j, data);
			k = i;
			while (--j >= 0)
				buf[j] = data->buf_str[(k--)];
			j = -1;
			data->lines[index_1]->cmnds[index_2][index_3 + count] = buf;
			// printf("NEW: %s at %d\n", data->lines[index_1]->cmnds[index_2][index_3 + count], index_3 + count);
		}
	}
	// WHY THE FUCK DONT I NEED TO FREE THE data->buf_str; when using valgrind and using return (free_str(data->buf_str), it says it is invalid free()); IN my opinion I have one malloc to much which is not freeed because I use the data->lines[index_1]->cmnds[index_2][index_3] at replace it with buf which is also malloced for;
	return (data->buf_str = NULL, count);
}



char *fill_normal(t_pipex *data, int index, int open)
{
	char *new;
	int char_count;
	// int empty_space;

	// printf("THIS: %d\n", index);
	// printf("FILL NORMAL: START: %s | J: %d \n", data->line + index, index);
	char_count = count_chars(data, index, open);
	// empty_space = check_for_empty(data, index + char_count);
	// printf("EMPTY SPACE %d\n", empty_space);
	new = malloc(sizeof(char) * (char_count + 1));
	if (!new)
		return (perror("malloc fail\n"), error_code(data), NULL);
	new[char_count] = 0;
	data->buf_int = open;
	ft_strncpy_2(new, data->line + index, char_count, data);
	// printf("NEW: %s\n", new);
	// while (--empty_space > 0)
	// 	new[empty_space] = ' ';
	return (new);
}

int dollar_in(char *line, int j, int open)
{
	// printf("START DOLLAR IN: |%s| OPEN: %d\n", line + j, open);
	// while (line[j])
	// {
	// 	if (is_quote_one(line[j]) && open == 1)
	// 		break;
	// 	if (line[j] == '$')
	// 		return (-1);
	// 	j++;
	// }
	// if (!line[j] || is_delim_front(line, j))
	// 	return (-1);
	// printf("DOLLAR 1: |%s| OPEN: %d\n", line + j, open);
	while (line[j])
	{
		// printf("DOLLAR 2: |%s| OPEN: %d\n", line + j, open);
		if (open == 1 && is_quote_one(line[j]) && is_delim_front(line, j + 1))
			break;
		if (open == 2 && is_quote_two(line[j]) && is_delim_front(line, j + 1))
			break;
		else if ((is_quote_two(line[j]) && open == 2) || (is_quote_one(line[j]) && open == 1))
			open = 0;
		else if (is_quote_two(line[j]) && !open)
			open = 2;
		else if (is_quote_one(line[j]) && !open)
			open = 1;
		// printf("DOLLAR 3: |%s| OPEN: %d\n", line + j, open);
		if (!open && (is_delim_front(line, j + 1)
			|| is_real_pipe(line, j + 1)
			|| (is_red_1(line[j]) && !is_red_1(line[j + 1]))
			|| (is_red_1(line[j + 1]) && !is_space(line[j]) && !is_red_1(line[j]))
			|| (is_space(line[j]) && is_quote(line[j + 1]))
			|| ((!line[j + 2] || is_space(line[j + 2]) || line[j + 2] == '\n') && is_quote(line[j + 1]))))
			break;
		// printf("DOLLAR 4: |%s| OPEN: %d\n", line + j, open);
		if (open != 1 && line[j] == '$' && line[j + 1] && (!is_space(line[j + 1]) && !(open == 2 && is_quote_two(line[j + 1]))))
			return (j);
			// return (printf("DOLLAR IN %d\n", j), j);
		j++;
	}
	// printf("DOLLAR IN is -1 \n");
	return (-1);
}

int		is_delim_back(char *line, int i)
{
	int open;

	open = 0;
	// printf("IS DELIM BACK??\n");
	if (i < 0 || !line[i])
		return (1);
	// printf("LNE: %s\n", line + i);
	while (is_quote(line[i]))
	{
		if ((open == 1 && is_quote_two(line[i])) || (open == 2 && is_quote_one(line[i])))
			return (0);
		if (is_quote_one(line[i]) && !open)
			open = 1;
		else if (is_quote_two(line[i]) && !open)
			open = 2;
		else if (((is_quote_one(line[i]) && open == 1) || (is_quote_two(line[i]) && open == 2)))
			open = 0;
		i--;
	}
	if (i < 0 || is_space(line[i]) || (is_red_1(line[i]) && line[i + 1] && !is_red_1(line[i + 1])) || is_red_clean(line, i) || is_real_pipe(line, i))
		return (1);
	// printf("NO DELIM\n");
	return (0);
}

int		is_delim_front(char *line, int i)
{
	int open;

	open = 0;
	while (line[i] && is_quote(line[i]))
	{
		if ((open == 1 && is_quote_two(line[i])) || (open == 2 && is_quote_one(line[i])))
			return (0);
		if (is_quote_one(line[i]) && !open)
			open = 1;
		else if (is_quote_two(line[i]) && !open)
			open = 2;
		else if (((is_quote_one(line[i]) && open == 1) || (is_quote_two(line[i]) && open == 2)))
			open = 0;
		i++;
	}
	if (!line[i] || line[i] == '\n' || is_space(line[i]) || is_red_clean(line, i) || is_real_pipe(line, i))
		return (1);
	return (0);
}

void	fill_for_empty(t_pipex *data, int index_1, int index_2, int index_3)
{

	data->lines[index_1]->cmnds[index_2][index_3] = malloc(sizeof(char) * (1));
	if (!data->lines[index_1]->cmnds[index_2][index_3])
		return (perror("malloc fail\n"), error_code(data));
	data->lines[index_1]->cmnds[index_2][index_3][0] = 0;
}

void	fill_cmnds(t_pipex *data, int index_1, int i, int j)
{
	int k;
	int	index_2;
	int open;

	index_2 = -1;
	open = 0;
	k = - i;
	// printf("START FILLING COMMANDS !!!!!!!!!!  K: %d\n", k);
	while (data->line[++j] && data->line[j] != '\n' && k <= 0)
	{
		// printf("FILL_CMNDS: 1 |%s| | OPEN: %d | K: %d | INDEX: %d \n", data->line + j, open, k, index_2);
		if (is_quote_one(data->line[j]) && !open)
			open = 1;
		else if (is_quote_two(data->line[j]) && !open)
			open = 2;
		else if (((is_quote_one(data->line[j]) && open == 1) || (is_quote_two(data->line[j]) && open == 2)))
			open = 0;
		// printf("FILL_CMNDS:     2 |%s| | OPEN: %d | K: %d | INDEX: %d J: %d\n", data->line + j, open, k, index, j);
		else if (k == 0 && !open && check_for_empty(data, j) && is_quote(data->line[j + 1]))
		{
			fill_for_empty(data, index_1, i, ++index_2);
			data->lines[index_1]->pos_in_line[i][index_2] = j;
		}
		else if (k == 0 && data->line[j]
			&& ((j == 0 && !is_quote(data->line[j])) || (j > 0 && (
					// (is_delim(data->line, j, open) && ((!is_space(data->line[j]) && !is_quote(data->line[j]) && !open) || open))
				// || (is_space(data->line[j - 1]) && !is_space(data->line[j]) && !open)
				// || (((j > 1 && (is_space(data->line[j - 2]) || is_real_pipe(data->line, j - 2) || is_red_clean(data->line, j - 2) || (open == 1))) || j < 2) 
				(is_red_clean(data->line, j) && !open)
					|| (open && j > 1 && is_space(data->line[j - 1]) && !is_space(data->line[j]))
					|| (!open && j > 1 && is_delim_back(data->line, j - 1) && !is_delim_back(data->line, j))
					|| (((j > 1 && is_delim_back(data->line, j - 2)) || j < 2)
						&& ((open == 1 && is_quote_one(data->line[j - 1]) && !is_quote_one(data->line[j])) || (open == 2 && is_quote_two(data->line[j - 1]) && !is_quote_two(data->line[j]))))
				|| (!open && !is_real_pipe(data->line, j) && !is_quote(data->line[j]) && !is_space(data->line[j]) && is_delim_back(data->line, j - 1) && !is_red_1(data->line[j]))
				|| (((is_red_in(data->line, j - 1) && is_red_out(data->line, j)) || (is_red_out(data->line, j - 1) && is_red_in(data->line, j))))
				|| (is_red_1(data->line[j - 1]) && !is_red_1(data->line[j]) && !is_space(data->line[j]) && data->line[j] != '|' && !open)
				|| ((is_real_pipe(data->line, j - 1)
					|| (j > 1 && data->line[j - 1] == '|' && data->line[j - 2] == '>')) && !is_space(data->line[j])))))
			&& ((dollar_in(data->line, j, open) >= 0 && count_expansion(data, j, open))
				|| dollar_in(data->line, j, open) == -1))
				// && (!open || ((open == 1 && is_quote_one(line[j - 1])) || (open == 2 && is_quote_two(line[j - 1])))))
		{
			// printf("TRUE! J : %d | I : %d\n", j, i);
			if (dollar_in(data->line, j, open) >= 0
				&& (index_2 == -1 || (index_2 >= 0 && ft_strncmp(data->lines[index_1]->cmnds[i][index_2], "<<", 3))))
				{
					data->lines[index_1]->cmnds[i][++index_2] = expand_it_1(data, j, open);
					index_2 += expand_it_2(data, index_1, i, index_2);
				}
			// else if (line[dollar_in(line, j, open) + 1] != '?')
			// 	arr[++index] = echo_exit_code();
			else
				data->lines[index_1]->cmnds[i][++index_2] = fill_normal(data, j, open);
			data->lines[index_1]->pos_in_line[i][index_2] = j;
			if ((open || dollar_in(data->line, j, open) >= 0) && (!ft_strncmp(data->lines[index_1]->cmnds[i][index_2], ">>", ft_strlen(data->lines[index_1]->cmnds[i][index_2]))
				|| !ft_strncmp(data->lines[index_1]->cmnds[i][index_2], "<<", ft_strlen(data->lines[index_1]->cmnds[i][index_2]))
				|| !ft_strncmp(data->lines[index_1]->cmnds[i][index_2], ">", ft_strlen(data->lines[index_1]->cmnds[i][index_2]))
				|| !ft_strncmp(data->lines[index_1]->cmnds[i][index_2], "<", ft_strlen(data->lines[index_1]->cmnds[i][index_2]))))
				{
					// printf("HE %s\n", data->lines[index_1]->cmnds[i][index_2]);
					data->lines[index_1]->red_cmnd[i][index_2] = 1;
				}
			if (!data->lines[index_1]->cmnds[i][index_2])
				error_code(data);
			// j += count_chars(data->line, j, open);
		}//   single quotes: 																// we have the beginning of the sandwich														// or the end of the sandwich
		// if ((open == 1 && is_quote_one(data->line[j]) && is_quote_one(data->line[j + 1]) && !((j > 0 && !is_space(data->line[j - 1])) || (data->line[j + 2] && !is_space(data->line[j + 2]))))
		// 	|| (open == 2 && is_quote_two(data->line[j]) && is_quote_two(data->line[j + 1]) && !((j > 0 && !is_space(data->line[j - 1])) || (data->line[j + 2] && !is_space(data->line[j + 2])))))
		// // if ((open == 1 && is_quote_one(data->line[j]) && is_quote_one(data->line[j + 1]) && (((j == 0 || is_space(data->line[j - 1])) && data->line[j + 2] && !is_space(data->line[j + 2])) || (!is_space(data->line[j - 1]) && (!data->line[j + 2] || is_space(data->line[j + 2])))))
		// // 	|| (open == 2 && is_quote_two(data->line[j]) && is_quote_two(data->line[j + 1]) && (((j == 0 || is_space(data->line[j - 1])) && data->line[j + 2] && !is_space(data->line[j + 2])) || (!is_space(data->line[j - 1]) && (!data->line[j + 2] || is_space(data->line[j + 2]))))))
		// {
		// 	printf("FILL NULL\n");
		// 	arr[++index] = malloc(sizeof(char) * (1));
		// 	arr[index][0] = 0;
		// }
		// printf("I1: %d\n", i);
		if (!data->line[j + 1] )
			break;
		if ((is_real_pipe(data->line, j) && j > 0 && !is_red_1(data->line[j - 1])))
			k++;
		// printf("I2: %d\n", i);
	}
}
