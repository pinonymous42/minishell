NAME = minishell
SRCDIR = ./
SRCS = destructer.c error.c expand.c main.c parse.c pipex.c tokenize.c
CC = gcc
CFLAGS = -lreadline
RM = rm -rf

# SRCS = $(addprefix $(SRCDIR), $(SRCNAMES))
OBJDIR = ./obj/
OBJS = $(SRCS:%.c=$(OBJDIR)%.o)

LIBDIR = ./libft
GNLDIR = ./libgnl

all: $(OBJDIR) $(NAME)

$(OBJDIR) :
	mkdir -p obj

$(OBJDIR)%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	make -C $(LIBDIR)
	make -C $(GNLDIR)
	$(CC) $(CFLAGS) -o $@ $^ -L $(LIBDIR) -L $(GNLDIR) -lft -lgnl

clean	:
	$(RM) $(OBJDIR)
	make -C $(LIBDIR) clean;
	make -C $(GNLDIR) clean;

fclean	:	clean
	make -C $(LIBDIR) fclean;
	make -C $(GNLDIR) fclean;
	$(RM) $(NAME)

re:	fclean all

.PHONY:		all clean fclean re
