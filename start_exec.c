#include "minishell.h"

int free_this(char *s)
{
    if (s)
        free(s);
    return (1);
}

int is_red_inline(t_pipex *data, int index_1, int index_2)
{
    int i;
    int check;

    i = -1;
    check = -1;
    while (data->lines[index_1]->cmnds[index_2] && data->lines[index_1]->cmnds[index_2][++i])
    {
        if (!ft_strncmp(data->lines[index_1]->cmnds[index_2][i], "<", 2))
            check = i;
        if (!ft_strncmp(data->lines[index_1]->cmnds[index_2][i], "<<", 3))
            check = i;
    }
    return (check);
}

int is_in_inline(t_pipex *data, int index_1, int index_2)
{// is  a single redirection in the line? 
    int i;
    int check;

    i = -1;
    check = -1;
    while (data->lines[index_1]->cmnds[index_2] && data->lines[index_1]->cmnds[index_2][++i])
    {
        if (!ft_strncmp(data->lines[index_1]->cmnds[index_2][i], "<", 2))
            check = i;
    }
    return (check);
}

char *replace_nl(char *str)
{
    int i;

    i = -1;
    while (str[++i])
    {
        if (str[i] == '\n')
            str[i] = 0;
    }
    return (str);
}

int is_valid_in(t_pipex *data, int index_1, int index_2)
{
    int i;
    int fd;

    i = -1;
    fd = -1;
    while (data->lines[index_1]->cmnds[index_2][++i])
    {
        if (!ft_strncmp(data->lines[index_1]->cmnds[index_2][i], "<", 2) && data->lines[index_1]->cmnds[index_2][i + 1]
            && data->lines[index_1]->red_cmnd[index_2][i] == 0)
        {
            close_pipe(data, fd);
            fd = open(data->lines[index_1]->cmnds[index_2][i + 1], O_RDONLY);
            if (fd == -1)
                return (perror("open"), 0);
        }
    }
    close_pipe(data, fd);
    return (fd);
}

int first_invalid_in(t_pipex *data, int index_1, int index_2)
{
    int i;
    int fd;

    i = -1;
    fd = -1;
    while (data->lines[index_1]->cmnds[index_2][++i])
    {
        if (!ft_strncmp(data->lines[index_1]->cmnds[index_2][i], "<", 2) && data->lines[index_1]->cmnds[index_2][i + 1]
            && data->lines[index_1]->red_cmnd[index_2][i] == 0)
        {
            close_pipe(data, fd);
            fd = open(data->lines[index_1]->cmnds[index_2][i + 1], O_RDONLY);
            if (fd == -1)
                return (perror("open"), 0);
        }
    }
    close_pipe(data, fd);
    return (0);
}

int first_invalid_out(t_pipex *data, int index_1, int index_2)
{
    int i;
    int fd;

    i = -1;
    fd = -1;
    while (data->lines[index_1]->cmnds[index_2][++i])
    {
        if ((!ft_strncmp(data->lines[index_1]->cmnds[index_2][i], ">>", 3) || !ft_strncmp(data->lines[index_1]->cmnds[index_2][i], ">", 2))
            && data->lines[index_1]->cmnds[index_2][i + 1] && data->lines[index_1]->red_cmnd[index_2][i] == 0)
        {
            close_pipe(data, fd);
            fd = open(data->lines[index_1]->cmnds[index_2][i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
                return (perror("open"), 0);
        }
    }
    close_pipe(data, fd);
    return (i);
}


int open_out(t_pipex *data, int index_1, int index_2)
{
    int i;
    int fd;

    i = -1;
    fd = 0;
    if (!is_valid_in(data, index_1, index_2) && first_invalid_in(data, index_1, index_2) < first_invalid_out(data, index_1, index_2))
        return (1);
    while (data->lines[index_1]->cmnds[index_2][++i])
    {
        if ((!ft_strncmp(data->lines[index_1]->cmnds[index_2][i], ">>", 3)
            || !ft_strncmp(data->lines[index_1]->cmnds[index_2][i], ">", 2)) && data->lines[index_1]->red_cmnd[index_2][i] == 0)
            {
                close_pipe(data, fd);
                if (!ft_strncmp(data->lines[index_1]->cmnds[index_2][i], ">>", 3))
                    fd = open(data->lines[index_1]->cmnds[index_2][i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    fd = open(data->lines[index_1]->cmnds[index_2][i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1)
                    return (printf("bash: %s: %s\n", data->lines[index_1]->cmnds[index_2][i + 1], strerror(errno)), exit_child(data, index_1, index_2, 1), -1);
            }
    }
    return (fd);
}


char *get_input_2(t_pipex *data, int index_1, int i)
{
    int k;
    int j;
    char *new;

    // printf("START GET INPUT 2\n");
    k = 0;
    j = 2;
    while (data->line[data->here_2 - j] && !is_delim_back(data->line, data->here_2 - j))
        j++;
    // printf("J: %d\n", j);
    // printf("data_here_2: %d | data_here_2_old: %d | J: %d\n", data->here_2, data->here_2_old, (data->here_2 - (j + 1) - data->here_2_old) + 1);
    (void)i;
    // while (data->lines[index_1]->cmnds[data->lines[index_1]->cmnd_count - 1][i])
    //     i++;
    (void)index_1;
    new = malloc(sizeof(char) * (((data->here_2 - (j + 1) - data->here_2_old) + 1) + 1));
    if (!new)
        return (perror("malloc failed!"), error_code(data), NULL);
    new[(data->here_2 - (j + 1) - data->here_2_old) + 1 + 1] = 0;
    while (k < (data->here_2 - (j + 1) - data->here_2_old) + 1 + 1)
    {
        new[k] = data->line[data->here_2_old + k];
        k++;
    }
    // data->buf_int = 2;
    // k = data->lines[index_1]->pos_in_line[data->lines[index_1]->cmnd_count - 1][i - 1];
    // i = 0;
    // while (data->line[k + i] != '\n')
    //     i++;
    // k += i + 1;
    // printf("K: %d\n", k);
    // i = 0;
    // while (data->line[data->here_2 - data->buf_int] && data->line[data->here_2 - data->buf_int] != '\n')
    //     data->buf_int++;
    // if (data->line[data->here_2 - data->buf_int] == '\n')
    //     data->buf_int--;
    // printf("BUF INT: %d\n", data->buf_int);
    // while (data->line[k + i] && k + i < data->here_2 - data->buf_int)
    //     i++;
    // // printf("I: %d\n", i);
    // new = ft_calloc(sizeof(char), (i + 1));
    // if (!new)
    //     return (perror("malloc failed!"), error_code(data), NULL);
    // new[i] = 0;
    // i = -1;
    // while (data->line[k + ++i] && k + i < data->here_2 - data->buf_int)
    //     new[i] = data->line[k + i];
    // data->buf_int = 0;
    // printf("NEW GET INPUT 2: %s\n", new);
    return (new);
}

int last_one(char **arr)
{
    int i;

    i = 0;
    while (arr[i])
        i++;
    if (i > 0)
        return (i - 1);
    return (0);
}

int find_first_here_doc(t_pipex *data, int index_1, int *index_2)
{
    int i;
    int j;
    int check;

    i = *index_2;
    check = -1;
    i = -1;
    while (check == -1 && data->lines[index_1]->cmnds[++i])
    {
        j = -1;
        while (data->lines[index_1]->cmnds[i][++j])
        {
            if (!ft_strncmp(data->lines[index_1]->cmnds[i][j], "<<", 3) && !data->lines[index_1]->red_cmnd[i][j])
            {
                check = j;
                break;
            }
        }
        j = 0;
    }
    return (check);
}

void set_data_here_2(t_pipex *data, int index_1)
{
    int i;
    int j;
    int check;

    i = -1;
    check = 0;
    while (!check && data->lines[index_1]->cmnds[++i])
    {
        j = -1;
        while (data->lines[index_1]->cmnds[i][++j])
        {
            if (!ft_strncmp(data->lines[index_1]->cmnds[i][j], "<<", 3) && !data->lines[index_1]->red_cmnd[i][j])
            {
                check = j;
                break;
            }
        }
        j = 0;
    }
    if (!check)
    {
        i = 0;
        while (data->line[data->lines[index_1]->pos_in_line[data->lines[index_1]->cmnd_count - 1][last_one(data->lines[index_1]->cmnds[data->lines[index_1]->cmnd_count - 1])] + i] 
            && data->line[data->lines[index_1]->pos_in_line[data->lines[index_1]->cmnd_count - 1][last_one(data->lines[index_1]->cmnds[data->lines[index_1]->cmnd_count - 1])] + i] != '\n')
            i++;
        // printf("SET DATA HERE & OLD: I: %d\n", data->lines[index_1]->pos_in_line[data->lines[index_1]->cmnd_count - 1][last_one(data->lines[index_1]->cmnds[data->lines[index_1]->cmnd_count - 1])] + i);
        data->here_2 = data->lines[index_1]->pos_in_line[data->lines[index_1]->cmnd_count - 1][last_one(data->lines[index_1]->cmnds[data->lines[index_1]->cmnd_count - 1])] + i + 1;
        data->here_2_old = data->lines[index_1]->pos_in_line[data->lines[index_1]->cmnd_count - 1][last_one(data->lines[index_1]->cmnds[data->lines[index_1]->cmnd_count - 1])] + i + 1;
    }
    if (check)
    // if (check && printf("SET_DATE_HERE & OLD: CALL FROM FIRST HERE_DOC I: %d | J: %d\n", i, check + 1))
        data->here_2_old = find_key(data, index_1, i, check + 1);
}

void start_exec(t_pipex *data, int index)
{
    int     i;
    int     status;
    pid_t   pid[data->lines[index]->cmnd_count];

    i = -1;
    create_pipes(data, index);
    // printf("INDEX %d\n", index);
    // signal(SIGCHLD, SIG_IGN);
    while (data->lines[index]->cmnds[++i] && here_doc(data, index , i))
    {
        // printf("OPS: %s\n", data->lines[index]->ops[i][0]);
        data->fd_out = open_out(data, index, i);
        if (ft_strncmp(data->lines[index]->paths[i], "pathnfound", 11)
            && is_valid_in(data, index, i) && data->fd_out >= 0 && data->lines[index]->ops[i][0][0])
        {
            if (check_here_doc(data, index, i) && free_this(data->input))
            {
                // if (printf("START_EXEC() CALL FROM IF I: %d | J: %d\n", i, is_red_inline(data, index, i) + 1) && find_key(data, index, i, is_red_inline(data, index, i) + 1))
                if (data->here_2_old < find_key(data, index, i, is_red_inline(data, index, i) + 1))
                {
                    // printf("START_EXEC(): SET HERE & OLD : CALL FROM INSIDE IF I: %d | J: %d\n", i, is_red_inline(data, index, i) + 1);
                    data->input = get_input_2(data, index, 0);
                    data->here_2_old = find_key(data, index, i, is_red_inline(data, index, i) + 1);
                }
                else
                    data->input = get_input(data, index, i, is_red_inline(data, index, i));
                // printf("INPUT: %s$\n", data->input);
            }
            if (!ft_strncmp(data->lines[index]->paths[i], "minicmnds", 11))
               exec_mini(data, index, i);
            else
                exec_cmnd(data, index, i, pid);
            // printf("_________________________________________________________________________________________\n");
        }
        else if (!is_valid_in(data, index, i) && !data->lines[index]->exit_codes[i])
        {
            printf("bash: %s: No such file or directory\n", data->lines[index]->cmnds[i][first_invalid_in(data, index, i)]);
            exit_child(data, index, i, 1);
        }
        else if (!ft_strncmp(data->lines[index]->paths[i], "pathnfound", 11) && data->lines[index]->ops[i][0] && !data->lines[index]->exit_codes[i])
        {
            printf("bash: command not found %s\n", data->lines[index]->ops[i][0]);
            exit_child(data, index, i, 127);
        }
        close_pipe(data, data->fd_out);
    }
    i = -1;
    close_pipes(data, index);
    while (++i < data->lines[index]->cmnd_count)
    {
        if (!data->lines[index]->exit_codes[i])
        {
            // printf("waiting..\n");
            waitpid(pid[i], &status, 0);
            if (WIFEXITED(status))
            {
                // printf("EXIT STATUS: %d\n", WEXITSTATUS(status));
                data->lines[index]->exit_codes[i] = WEXITSTATUS(status);
            }
        }
    }
    set_data_here_2(data, index);
    data->last_exit_status = data->lines[index]->exit_codes[data->lines[index]->cmnd_count - 1];
    // printf("LAST1: %d\n", data->exit_codes[data->cmnd_count - 1]);
    // exit_child(data, data->cmnd_count - 1, data->exit_codes[data->cmnd_count - 1]);
    // printf("LAST2: %d\n", data->exit_codes[data->cmnd_count - 1]);
}
