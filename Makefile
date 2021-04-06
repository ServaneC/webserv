# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: schene <schene@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/25 14:21:19 by schene            #+#    #+#              #
#    Updated: 2021/04/01 15:28:41 by schene           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

SRCSDIR		=  srcs

OBJDIR		= .obj

SRCS		= Server.cpp main.cpp Request.cpp myCGI.cpp Response.cpp

OBJS		= $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

DPDCS		= $(OBJS:.o=.d)

INCLUDES = -I include/ -I libft/include/

LIB = libft/libft.a

CFLAGS		= -Wall -Wextra -Werror -std=c++98

LIBFT_FLAGS = 	-Llibft -lft

RM			= rm -f

CC			= clang++

opti:
	@make -j all

all : 
	@printf "[libft] " && make -C libft
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
			@(make clean -C libft)

cleanlib :
			@(make fclean -C libft)

fclean :	cleanlib clean
			@(rm -rf $(NAME))
			@(rm -f libft.a)
			@(rm -rf a.out.dSYM)
			@(rm -f a.out)

re :
			@make fclean
			@make opti
			

