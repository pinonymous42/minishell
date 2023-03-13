NAME = minishell
SRCDIR = ./srcs/
SRCS = 0_main.c 1_make_environ.c 2_tokenize.c 3_1_expand.c 3_2_token_list_to_array.c 3_3_search_env.c 4_pipe.c \
		 5_signal.c 6_1_exit_builtin.c 6_2_env_builtin.c 6_3_echo_builtin.c 6_4_cd_builtin.c 6_5_pwd_builtin.c \
		 6_6_export_builtin.c 6_7_unset_builtin.c 7_destructer.c 8_error.c
# SRCS = token_list_to_array.c destructer_ref.c error_ref.c expand_ref.c ../test.c tokenize_ref.c pipe.c \
# 		make_environ.c signal.c search_env.c cd_builtin

CC = gcc
CFLAGS = -I $(shell brew --prefix readline)/include -Wall -Wextra -Werror
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
	echo > ~/.inputrc set echo-control-characters off

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