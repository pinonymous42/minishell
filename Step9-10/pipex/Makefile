NAME = pipex
CC = gcc
CFLAGS =
RM = rm -rf

SRCNAMES = main.c
SRCDIR = src/
SRCS = $(addprefix $(SRCDIR), $(SRCNAMES))
OBJS = $(SRCS:%.c=%.o)

LIBDIR = ./libft
GNLDIR = ./libgnl

all: $(NAME)

$(NAME): $(OBJS)
	make -C $(LIBDIR)
	make -C $(GNLDIR)
	$(CC) $(CFLAGS) -o $@ $^ -L $(LIBDIR) -L $(GNLDIR) -lft -lgnl

clean	:
	$(RM) $(OBJS)
	$(RM) $(OBJS_B)
	make -C $(LIBDIR) clean;
	make -C $(GNLDIR) clean;

fclean	:	clean
	make -C $(LIBDIR) fclean;
	make -C $(GNLDIR) fclean;
	$(RM) $(NAME)

re:	fclean all

.PHONY:		all clean fclean re