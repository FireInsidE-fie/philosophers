NAME		=	philosophers
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g
RM			=	rm
RMFLAGS		=	-f

SRCDIR		=	./src
CFILES		=	$(SRCDIR)/philosophers.c \
OBJS		=	$(CFILES:.c=.o)

INCLDIR		=	include/
IFILES		=	$(INCLDIR)philosophers.h

# Link GNU readline on my mac
UNAME_S		= 	$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
$(info MacOS detected, linking with custom GNU Readline...)
LIBS		+=	-L/opt/homebrew/opt/readline/lib
endif

all:			$(NAME)

%.o:			%.c $(IFILES)
				@printf "\rCompiling $<..."
				@$(CC) $(CFLAGS) -I$(INCLDIR) -c $< -o $@

# Compile minishell with the existing libs
$(NAME):		$(OBJS)
				@printf "\rCompiling $(NAME)..."
				@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
				@printf "\r\n\033[32m$(NAME) compiled.\033[0m\n"
clean:
				@printf "\rCleaning object files"
				@$(RM) $(RMFLAGS) $(OBJS)
				@make clean -C $(LIBFT_DIR)/ --no-print-directory
				@printf "\rObject files cleaned.\n"

fclean:			clean
				@printf "\rRemoving $(NAME)..."
				@$(RM) $(RMFLAGS) $(NAME)
				@make fclean -C $(LIBFT_DIR)/ --no-print-directory
				@printf "\r$(NAME) Removed.\n"

re:				fclean all

.PHONY:			all clean fclean re
