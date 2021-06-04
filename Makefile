# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: schene <schene@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/25 14:21:19 by schene            #+#    #+#              #
#    Updated: 2021/04/30 10:09:26 by schene           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

SRCSDIR		=  srcs

OBJDIR		= .obj

SRCS		= Server.cpp main.cpp Request.cpp execCGI.cpp Response.cpp Config.cpp

OBJS		= $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

DPDCS		= $(OBJS:.o=.d)

INCLUDES = -I include/

CFLAGS		= -Wall -Wextra -Werror -std=c++98 -fsanitize=address

RM			= rm -f

CC			= clang++

opti:
	@make -j all

all : 
	@make $(NAME)

-include $(DPDCS)

$(NAME) : $(OBJS) $(LIB)
	@printf "[$(NAME)] "
	@${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIB}
	@echo Compiled $(NAME) successfully !
	@echo $(NAME) > .gitignore

#${NAME}:	${OBJS} $(LIB)
#			@echo "✅"
#			@printf "[libft] " && make -C libft
#			@printf "[$(NAME)] "
#			@${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIB}
#			@echo "✅ \033[32mCompilation Done, ./${NAME} created.\033[0m"

$(OBJDIR) :
			@mkdir -p .obj 

$(OBJDIR)/%.o : $(SRCSDIR)/%.cpp | $(OBJDIR)
			@${CC} ${CFLAGS} -c $< -o $@

clean :
			@(rm -rf $(OBJDIR))

fclean :	clean
			@(rm -rf $(NAME))
			@(rm -rf a.out.dSYM)
			@(rm -f a.out)

re :
			@make fclean
			@make opti
			

