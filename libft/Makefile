NAME = libft.a

SRCDIR = srcs

OBJDIR = .obj

SRCS =	ft_atoi.c	\
		ft_bzero.c	\
		ft_calloc.c	\
		ft_isalnum.c	\
		ft_isalpha.c	\
		ft_isascii.c	\
		ft_isdigit.c	\
		ft_isprint.c	\
		ft_itoa.c	\
		ft_memccpy.c	\
		ft_memchr.c	\
		ft_memcmp.c	\
		ft_memcpy.c	\
		ft_memmove.c	\
		ft_memset.c	\
		ft_putchar_fd.c	\
		ft_putendl_fd.c	\
		ft_putnbr_fd.c	\
		ft_putstr_fd.c	\
		ft_split.c	\
		ft_strchr.c	\
		ft_strrchr.c	\
		ft_strdup.c	\
		ft_strjoin.c	\
		ft_strlcat.c	\
		ft_strlcpy.c	\
		ft_strlen.c	\
		ft_strmapi.c	\
		ft_strncmp.c	\
		ft_strnstr.c	\
		ft_strtrim.c	\
		ft_substr.c	\
		ft_tolower.c	\
		ft_toupper.c	\
		get_next_line.c \
		seek_nl.c	\
		new_str_from.c \
		trim_spaces.c \
		spaceless_strlen.c \
		ft_sort_int_tab.c \
		ft_swap.c \
		ft_sort_double_tab.c \
		ft_swap_double.c \
		ft_rev_int_tab.c \
		ft_rev_double_tab.c \
		ft_isspace.c \
		ft_iswhitespace.c \
		ft_atoi_ptr.c \
		ft_abs.c \
		ft_atoi_from.c \
		ft_lstadd_back.c \
		ft_lstadd_front.c \
		ft_lstdelone.c \
		ft_lstclear.c \
		ft_lstnew.c \
		ft_lstclear.c \
		ft_lstsize.c \
		ft_lstiter.c \
		ft_lstlast.c \
		ft_lstmap.c \
		ft_putstr.c

OBJS = 	$(addprefix $(OBJDIR)/,$(SRCS:.c=.o))

DPDCS = $(OBJS:.o=.d)

INCLUDES = -I include/

CFLAGS = 	-Wall -Wextra -Werror

.PHONY = all clean fclean re

opti:
	@make -j all

all : $(NAME)

$(NAME):	$(OBJS)
	@(ar crs $(NAME) $(OBJS))
	@echo Compiled $(NAME) successfully !

$(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)
	@clang $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(OBJDIR) :
	@mkdir -p .obj


clean:
	@(rm -rf $(OBJDIR))

fclean: clean
	@(rm -f $(NAME))

re :
	@make fclean
	@make opti
