/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhocsak <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:41:53 by bhocsak           #+#    #+#             */
/*   Updated: 2024/10/11 15:41:55 by bhocsak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// TAKE THAT OUT
void	print_that_shit(t_pipex *data)
{
	int	i;
	int	j;

	i = -1;
	while (data->cmnds[++i])
	{
		j = -1;
		while (data->cmnds[i][++j])
			printf("ELEM: %d:%d | %s\n", i, j, data->cmnds[i][j]);
		j = -1;
		if (data->ops[i])
		while (data->ops[i][++j])
			printf("OP:   %d:%d | %s\n", i, j, data->ops[i][j]);
		printf("PATH: %s\n", data->paths[i]);
		if (i > 10)
			break;
	}
	// i = -1;
	// while (data->mini_env[++i])
	// 	printf("ENV[%d]: %s\n", i, data->mini_env[i]);
}

void	init_ops(t_pipex *data, int cmnd_count)
{
    int i;

    i = -1;
    data->ops = malloc(sizeof(char **) * (cmnd_count + 1));
    if (!data->ops)
        return (perror("malloc fail!\n"), error_code(data, NULL, 1, errno));
    data->ops[cmnd_count] = 0;
    while (data->cmnds[++i])
    {
        data->ops[i] = malloc(sizeof(char *) * (count_ops(data, i) + 1));
        if (!data->ops[i])
            return (perror("malloc fail!\n"), error_code(data, NULL, 1, errno));
        data->ops[i][count_ops(data, i)] = 0;
        fill_ops(data, i);
    }
}

void	init_cmds(t_pipex *data, char *line, int count)
{
	int	i;

	i = -1;
	data->cmnds = malloc(sizeof(char **) * (count + 1));
	if (!data->cmnds)
		return (perror("malloc fail!\n"), error_code(data, line, 0, 0));
	data->cmnds[count] = NULL;
	while (++i < count)
	{
		data->cmnds[i] = malloc(sizeof(char *) * (count_elem(line, i, 0) + 1));
		if (!data->cmnds[i])
			return (perror("malloc fail!\n"), error_code(data, line, 0, 0));
		data->cmnds[i][count_elem(line, i, 0)] = NULL;
		data->cmnds[i] = fill_cmnds(data->cmnds[i], line, i, -i);
		if (!data->cmnds[i])
			return (perror("malloc fail!\n"), error_code(data, line, 0, 0));
		if (!data->cmnds[i])
			return (error_code(data, line, 0, 0));
	}
}

void	init_paths(t_pipex *data, int count)
{
	int	i;

	i = -1;
	data->paths = malloc(sizeof(char *) * (count + 1));
	if (!data->paths)
		return (perror("malloc fail!\n"), error_code(data, NULL, 0, errno));
	data->paths[count] = NULL;
	while (++i < count)
	{
		if (data->ops[i][0] && ft_strncmp(data->ops[i][0], "cd", 3)
			&& ft_strncmp(data->ops[i][0], "env", 4) && ft_strncmp(data->ops[i][0], "export", 7))
		{
			data->paths[i] = find_path(data->cur_env, data->ops[i][0]);
			if (!data->paths[i])
			{
				data->paths[i] = ft_strdup("pathnfound");
				printf("zsh: command not found %s\n", data->ops[i][0]);
				exit_child(127, NULL, data);
			}
		}
		else
			data->paths[i] = ft_strdup("pathnfound");
		if (!data->paths[i])
			return (perror("malloc fail!\n"), error_code(data, NULL, 0, errno));
	}
}

void	parsing(t_pipex *data, char *line, char **env)
{
	char	*trimmed;
	int		cmnd_count;

	(void)env;
	trimmed = ft_strtrim(line, " \n\t\f\v\r");
	if (!trimmed)
		return (rl_clear_history(), free_list(env), error_code(data, line, 1, 1));
	free(line);
	line = trimmed;
	if (!syntax_check(line, -1, 0))
		return (perror("bash: syntax error near unexpected token `|'"), error_code(NULL, line, 0, errno));
	cmnd_count = count_cmnds(line);
	printf("COUNT CMND_LINES%d\n", cmnd_count);
	data->paths = NULL;
	data->cmnds = NULL;
	data->ops = NULL;
	data->input = NULL;
	printf("THIS: $%s$\n", line);
	// malloc failing testing till here fine
	while (check_open(line))
	{
		line = join_this(join_this(line, "\n"), get_next_line(0, 0));
		//if (!line)
		//	return (rl_clear_history(), free_list(env), error_code(data, NULL, 1, 1));
	}
	init_cmds(data, line, cmnd_count);
	printf("ENV3: %p\n", data->cur_env);
	if (!check_reds(data))
		return (free_struct(data));
	return (set_cur_path(data), init_ops(data, cmnd_count), init_paths(data, cmnd_count),
		print_that_shit(data), start_exec(data), free_struct(data));
}
//	return (rl_clear_history(), free_list(env), error_code(data, line, 1, 1));