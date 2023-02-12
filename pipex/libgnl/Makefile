NAME = libgnl.a

SRCDIR = ./
SRCNAMES = get_next_line.c get_next_line_utils.c

CFLAGS = -Wall -Wextra -Werror
CC = gcc

OBJDIR = ./obj/
OBJS = $(SRCNAMES:%.c=$(OBJDIR)%.o)

all : $(OBJDIR) $(NAME)

$(OBJDIR) :
	mkdir -p obj

$(OBJDIR)%.o : $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME):	$(OBJS)
		ar rcs $@ $^

clean :
	rm -fr $(OBJDIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re