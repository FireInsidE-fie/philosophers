NAME		=	philosophers
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -std=c11 -g
RM			=	rm
RMFLAGS		=	-f

SRCDIR		=	src
CFILES		=	$(SRCDIR)/philosophers.c \
				$(SRCDIR)/free_thinkers/thinkers.c \
				$(SRCDIR)/safe_utils.c \
				$(SRCDIR)/utils.c
OBJS		=	$(CFILES:.c=.o)

INCLDIR		=	include/
IFILES		=	$(INCLDIR)philosophers.h

LIBS		=	-lpthread


all:			$(NAME)

%.o:			%.c $(IFILES)
				@printf "\rCompiling $<..."
				@$(CC) $(CFLAGS) -I$(INCLDIR) -c $< -o $@

$(NAME):		$(OBJS)
				@printf "\rCompiling $(NAME)..."
				@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
				@printf "\r\n\033[32m$(NAME) compiled.\033[0m\n"

clean:
				@printf "\rCleaning object files"
				@$(RM) $(RMFLAGS) $(OBJS)
				@printf "\rObject files cleaned.\n"

fclean:			clean
				@printf "\rRemoving $(NAME)..."
				@$(RM) $(RMFLAGS) $(NAME)
				@printf "\r$(NAME) Removed.\n"

re:				fclean all

.PHONY:			all clean fclean re
