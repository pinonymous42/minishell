NAME = minishell
SRCDIR = ./srcs/
SRCS = token_list_to_array.c destructer_ref.c error_ref.c expand_ref.c main_ref.c tokenize_ref.c pipe_ref.c \
		make_environ.c signal.c search_env.c

CC = gcc
CFLAGS = -I $(shell brew --prefix readline)/include
LDFLAGS = -lreadline -lhistory -L$(shell brew --prefix readline)/lib
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
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -L $(LIBDIR) -L $(GNLDIR) -lft -lgnl

clean	:
	$(RM) $(OBJDIR)
	make -C $(LIBDIR) clean;
	make -C $(GNLDIR) clean;

fclean	:	clean
	make -C $(LIBDIR) fclean;
	make -C $(GNLDIR) fclean;
	$(RM) $(NAME)

re:	fclean all

test: all
	./test.sh
	
.PHONY:		all clean fclean re