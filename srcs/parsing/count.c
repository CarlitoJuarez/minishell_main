#include "../../minishell.h"

int count_lines(char *line)
{
	int i;
	int count;

	i = -1;
	count = 1;
	while (line[++i])
	{
		if (line[i] == '\n' && line[i + 1])
			count++;
	}
	return (count);
}

int	count_cmnds(char *line, int index)
{
	int	count;
	int	i;
	int open;

	count = 1;
	i = -1;
	open = 0;
	if (!line || !*line)
		return (0);
	// while (index > 0 && line[++i])
	// {
	// 	if (line[i] == '\n')
	// 		index--;
	// }
	(void)index;
	while (line && line[++i] && line[i] != '\n')
	{
		// printf("COUNT _ CMNDS : %s\n    COUNT: %d  |  OPEN: %d\n", line + i, count, open);
		if (is_quote_one(line[i]) && ((i > 0 && is_space(line[i - 1])) || i == 0) && !open)
			open = 1;
		else if (is_quote_two(line[i]) && ((i > 0 && is_space(line[i - 1])) || i == 0) && !open)
			open = 2;
		else if (((is_quote_one(line[i]) && open == 1) || (is_quote_two(line[i]) && open == 2)))
			open = 0;
		if (is_real_pipe(line, i) && !open)
			count++;
		if (!line[i])
			break;
	}
	// printf("COUNT CMNDS: %d\n", count);
	return (count);
}

int	count_elem(t_pipex *data, int index_1, int i, int j)
{
	int	k;
	int open;

	k = -i;
	open = 0;
	data->count_elem = 0;
	(void)index_1;
	// printf("START counting ELEMS %s\n", data->line + j + 1);
	while (data->line[++j] && data->line[j] != '\n' && i >= 0)
	{
		// if (data->line[j] == '\n')
		// 	index_1--;
		if (k == 0)
		{
			// printf("LINE: 1 |%s| COUNT: %d OPEN: %d  | J: %d\n", data->line + j, data->count_elem, open, j);
			if (is_real_pipe(data->line, j) && !open)
				break ;
			// if (is_quote_one(data->line[j]) && !open && (is_space(data->line[j - 1]) || is_real_pipe(data->line, j -1) || is_red_clean(data->line, j - 1)) && count++)
			// 	open = 1;
			// else if (is_quote_two(data->line[j]) && !open && (is_space(data->line[j - 1]) || is_real_pipe(data->line, j -1) || is_red_clean(data->line, j - 1)) && count++)
			// 	open = 2;
			// else if (is_quote_one(data->line[j]) && !open && !is_space(data->line[j - 1]))
			// 	open = 1;
			// else if (is_quote_two(data->line[j]) && !open && !is_space(data->line[j - 1]))
			// 	open = 2;
			// else if (((is_quote_one(data->line[j]) && open == 1) || (is_quote_two(data->line[j]) && open == 2)))
			// 	open = 0;
			if (is_quote_one(data->line[j]) && !open)
				open = 1;
			else if (is_quote_two(data->line[j]) && !open)
				open = 2;
			else if (((is_quote_one(data->line[j]) && open == 1) || (is_quote_two(data->line[j]) && open == 2)))
				open = 0;
			// else if (!open && data->line[j]
			// 	&& (open == 1 || ((dollar_in(data->line, j, open) >= 0 && count_expansion(data, j, open, data->cur_env))
			// 		|| dollar_in(data->line, j, open) == -1))
			// 	&& ((j == 0 && !is_space(data->line[j]))
			// 	|| (is_delim(data->line, j) && !is_space(data->line[j]))
			// 	|| (is_red_clean(data->line, j) && !is_red_clean(data->line, j - 1))
			// 	|| ((is_red_in(data->line[j - 1]) && is_red_out(data->line[j])) || (is_red_out(data->line[j - 1]) && is_red_in(data->line[j])))
			// 	|| (is_red_1(data->line[j - 1]) && !is_space(data->line[j]) && data->line[j] != '|' && !is_red_1(data->line[j]))
			// 	|| ((is_real_pipe(data->line, j - 1)
			// 		|| (data->line[j - 1] == '|' && data->line[j - 2] == '>')) && !is_space(data->line[j]))))
			// printf("LINE:   2 |%s| COUNT: %d OPEN: %d | J: %d\n", data->line + j, count, open, j);
			else if (data->line[j]
			&& ((j == 0 && !is_quote(data->line[j])) || (j > 0 && (
					// (is_delim(data->line, j, open) && ((!is_space(data->line[j]) && !is_quote(data->line[j]) && !open) || open))
				// || (is_space(data->line[j - 1]) && !is_space(data->line[j]) && !open)
				// || (((j > 1 && (is_space(data->line[j - 2]) || is_real_pipe(data->line, j - 2) || is_red_clean(data->line, j - 2) || (open == 1))) || j < 2) 
					(is_red_clean(data->line, j) && !open)
					|| (!open && check_for_empty(data, j) && is_quote(data->line[j + 1]))
					|| (!open && j > 1 && is_delim_back(data->line, j - 1) && !is_delim_back(data->line, j))
					|| (open && j > 1 && is_space(data->line[j - 1]) && !is_space(data->line[j]))
					|| (((j > 1 && is_delim_back(data->line, j - 2)) || j < 2)
						&& ((open == 1 && is_quote_one(data->line[j - 1]) && !is_quote_one(data->line[j])) || (open == 2 && is_quote_two(data->line[j - 1]) && !is_quote_two(data->line[j]))))
					|| (!open && !is_real_pipe(data->line, j) && !is_quote(data->line[j]) && !is_space(data->line[j]) && is_delim_back(data->line, j - 1) && !is_red_1(data->line[j]))
					|| ((is_red_in(data->line, j - 1) && is_red_out(data->line, j)) || (is_red_out(data->line, j - 1) && is_red_in(data->line, j)))
					|| (is_red_1(data->line[j - 1]) && !is_red_1(data->line[j]) && !is_space(data->line[j]) && data->line[j] != '|' && !open)
					|| ((is_real_pipe(data->line, j - 1)
						|| (data->line[j - 1] == '|' && data->line[j - 2] == '>')) && !is_space(data->line[j])))))
			&& ((dollar_in(data->line, j, open) >= 0 && count_expansion(data, j, open))
				|| dollar_in(data->line, j, open) == -1))
					data->count_elem++;
		}
		else if (is_real_pipe(data->line, j) && --i != INT_MIN)
			k++;
	}
	// printf("COUNT ELEM RETURN: %d\n", data->count_elem);
	return (data->count_elem);
}

int check_for_empty(t_pipex *data, int i)
{
	if (!data->line[i] || !is_space(data->line[i]))
		return (0);
	while (data->line[i] && is_space(data->line[i]))
		i++;
	if (data->line[i] && is_quote_one(data->line[i])
		&& data->line[i + 1] && is_quote_one(data->line[i + 1])
		&& ((data->line[i + 2] && (is_space(data->line[i + 2]) || is_real_pipe(data->line, i + 2) || data->line[i + 2] == '\n'))
			|| (!data->line[i + 2])))
		return (i += 2, check_for_empty(data, i) + 1);
	else if (data->line[i] && is_quote_two(data->line[i])
		&& data->line[i + 1] && is_quote_two(data->line[i + 1])
		&& ((data->line[i + 2] && (is_space(data->line[i + 2]) || is_real_pipe(data->line, i + 2) || data->line[i + 2] == '\n'))
			|| (!data->line[i + 2])))
		return (i += 2, check_for_empty(data, i) + 1);
	else
		return (0);
}

int	count_chars(t_pipex *data, int i, int open)
{
	int count;

	count = 0;
	// printf("START COUNTING CHARS.. | OPEN %d\n", open);
	while (data->line[i])
	{
		// printf("LINE COUNT_CHARS 1: |%s| | OPEN: %d | COUNT: %d\n", data->line + i, open,count);
		// else if (is_quote_one(line[i]) && !open && !is_space(line[i - 1]))
		// 	open = 1;
		// else if (is_quote_two(line[i]) && !open && !is_space(line[i - 1]))
		// 	open = 2;
		// else if (is_space(line[i + 1]) && ((is_quote_one(line[i]) && open == 1) || (is_quote_two(line[i]) && open == 2)))
		// 	break;
		// printf("LINE COUNT_CHARS 2 2 2: %s | OPEN: %d | COUNT: %d\n", line + i, open,count);
		// if (!open && (!line[i + 1] || is_space(line[i + 1]) || line[i + 1] == '\n'
		// 	|| is_real_pipe(line, i + 1)
		// 	|| (is_red_1(line[i]) && !is_red_1(line[i + 1]))
		// 	|| ((is_red_in(line[i + 1]) && is_red_out(line[i])) || (is_red_out(line[i + 1]) && is_red_in(line[i])))
		// 	|| (is_red_1(line[i + 1]) && !is_space(line[i]) && !is_red_1(line[i]))))
		// 	// || ((!line[i + 2] || is_space(line[i + 2]) || line[i + 2] == '\n') && is_quote(line[i + 1]))))
		// 	break;
		// else if ((!line[i + 2] || (line[i + 2] && (is_space(line[i + 2]) || line[i + 2] == '\n' || line[i + 2] == '|' || is_red_clean(line, i + 2))))
		// 	&& ((is_quote_one(line[i + 1]) && open == 1) || (is_quote_two(line[i + 1]) && open == 2) || (is_quote(line[i + 1]) && !open)))
		// 		break ;
		// // else if (((line[i + 2] && (is_space(line[i + 2]) || line[i + 2] == '\n' || line[i + 2] == '|' || is_red_clean(line, i + 2)))
		// // 	&& ((open == 1 && is_quote_one(line[i + 1])) || (open == 2 && is_quote_two(line[i + 1])))))
		// // 		break ;
		// // if (!(!open && i > 0 && !is_space(line[i - 1]) && (is_quote(line[i]))))
		// if (line[i + 1] && ((!is_quote(line[i + 1])) || ((open == 1 && is_quote_two(line[i + 1])) || (open == 2 && is_quote_one(line[i + 1])))))
		// 	count++;
		if (count > 0 && is_quote_one(data->line[i]) && open == 1 && ((data->line[i + 1] && is_delim_front(data->line, i + 1)) || !data->line[i + 1]))
			break;
		else if (count > 0 && is_quote_two(data->line[i]) && open == 2 && ((data->line[i + 1] && is_delim_front(data->line, i + 1)) || !data->line[i + 1]))
			break;
		else if (count > 0 && !open && is_quote(data->line[i]) && is_delim_back(data->line, i - 1))
			break;
		else if (count > 0 && open && is_delim_back(data->line, i))
			break;
		else if (is_quote_one(data->line[i]) && !open)
			open = 1;
		else if (is_quote_two(data->line[i]) && !open)
			open = 2;
		else if ((is_quote_one(data->line[i]) && open == 1) || (is_quote_two(data->line[i]) && open == 2))
			open = 0;
		// printf("LINE COUNT_CHARS    2: |%s| | OPEN: %d | COUNT: %d\n", data->line + i, open,count);
		// else if ((!open || (is_delim_back(data->line, i - 1) && count > 0)) && (!data->line[i] || is_space(data->line[i]) || data->line[i] == '\n'
		else if (!open && (!data->line[i] || is_space(data->line[i]) || data->line[i] == '\n'
			|| is_real_pipe(data->line, i)
			|| (count > 0 && i > 0 && is_red_1(data->line[i - 1]) && !is_red_1(data->line[i]) && (is_red_clean(data->line, i - 1) || (i > 1 && is_red_clean(data->line, i - 2))))
			// || (count == 1 && i > 0 && is_red_clean(data->line, i - 1) && !is_red_clean(data->line, i))
			// || (count == 2 && i > 1 && is_red_clean(data->line, i - 2) && !is_red_clean(data->line, i - 1))
			// || (i > 0 && is_red_clean(data->line, i - 1) && count == 1)
			// || (i > 1 && is_red_clean(data->line, i - 2) && count == 2)
			|| (data->line[i + 1] && (is_space(data->line[i]) && is_quote(data->line[i + 1])))
			|| (data->line[i + 1] && (!data->line[i + 2] || is_space(data->line[i + 2]) || data->line[i + 2] == '\n') && is_quote(data->line[i + 1]))))
			break;
		if (data->line[i] && ((!is_quote(data->line[i])) || ((open == 1 && is_quote_two(data->line[i]))
			|| (open == 2 && is_quote_one(data->line[i])))))
			count++;
		i++;
	}
	// count += check_for_empty(data, i);
	// printf("COUNT CHARS RETURN: %d\n", count);
	return (count);
}

int	count_chars_2(t_pipex *data, int i)
{
	int count;

	count = 0;
	// printf("START COUNTING CHARS 02\n");
	while (data->line[i])
	{
		// printf("LINE: %s COUNT: %d\n", data->line + i, count);
		if (!data->line[i] || is_space(data->line[i]) || data->line[i] == '\n'
			|| is_real_pipe(data->line, i)
			|| is_red_clean(data->line, i)
			|| data->line[i] == '/'
			|| (data->line[i] == '$' && count > 1)
			|| is_quote(data->line[i]))
			break;
		count++;
		i++;
	}
	// count += check_for_empty(data, i);
	// printf("COUNT CHARS 02 RETURN: %d\n", count);
	return (count);
}