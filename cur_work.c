#include "../minishell.h"

bool	is_special(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '&');
}

t_line	*new(char *cmd, bool isopp)
{
	t_line	*new;

	new = malloc(sizeof(t_line));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->cmd = cmd;
	new->isopp = isopp;
	return (new);
}

t_line	*get_last(t_line *head)
{
	if (head == NULL)
		return (NULL);
	if (head->next == NULL)
		return (head);
	return get_last(head->next);
}

void	append(t_line **head, t_line *new)
{
	t_line	*h;

	h = *head;
	if (*head == NULL)
		*head = new;
	while (h->next)
	{
		h = h->next;
	}
	h->next = new;
}

void	token(t_line **line, char *cmd, bool isopp)
{
	t_line	*l;
	t_line	*last;

	if (isopp)
	{
		l = new(cmd, isopp);
		append(line, l);
	}
	else
	{
		last = get_last(*line);
		if (last == NULL || last->isopp)
		{
			l = new(cmd, isopp);
			append(line, l);
		}
		else
		{
			
		}
	}
}

t_line	*init_line(char *cmd)
{
	char	**cmds;
	int		i;
	t_line	*line;

	i = 0;
	line = NULL;
	cmds = ft_split(cmd, ' ');
	while (cmds[i] != NULL)
	{
		if (is_special(*cmds[i]))
		{
			token(&line, cmds[i], true);
			i++;
		}
		else
			token(&line, cmds[i], false);
	}
}

int	try(char *cmds, t_var *var)
{
	t_line *line = init_line(cmds);
	if (!cmds || !var->env)
	{
		if (cmds)
			free(cmds);
		return (-2);
	}
	if (ft_strcmp(cmds[0], "cd") == 0)
	{
		if (cmds[1] != NULL)
			cd(cmds[1], var);
		else
			cd(NULL, var);
	}
	else if (ft_strcmp(cmds[0], "pwd") == 0)
		pwd();
	else if (ft_strcmp(cmds[0], "env") == 0)
		env(var->env);
	else if (ft_strcmp(cmds[0], "clear") == 0)
		clear();
	else if (ft_strcmp(cmds[0], "echo") == 0)
	{
		if (ft_strcmp(cmds[1], "-n") == 0)
			echo(cmds[2], false, var);
		else
			echo(cmds[1], true, var);
	}
	else if (ft_strcmp(cmds[0], "export") == 0)
		export(cmds[1], var);
	else if (ft_strcmp(cmds[0], "unset") == 0)
		unset(cmds[1], var);
	return (0);
}
