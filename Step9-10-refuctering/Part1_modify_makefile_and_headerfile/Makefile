NAME = minishell
SRCDIR = ./srcs/
SRCS = destructer.c error.c expand.c main.c parse.c pipe.c tokenize.c

CC = gcc
CFLAGS = -lreadline
RM = rm -rf

OBJDIR = ./obj/
OBJS = $(addprefix $(OBJDIR), $(SRCS:.c=.o))

LIBDIR = ./libft
GNLDIR = ./libgnl

all: $(OBJDIR) $(NAME)

$(OBJDIR) :
	mkdir -p obj

$(OBJDIR)%.o : $(SRCDIR)%.c
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