NAME = pipex
S_DIR = srcs
H_DIR = includes
SRCS_FILES = main.c \
				utils.c
SRCS = $(addprefix ${S_DIR}/, ${SRCS_FILES})
OBJS = ${SRCS:.c=.o}
LFT = lib/libft
LIBFLAGS = -Llib/libft -lft
FLAGS = -Wall -Wextra -Werror -I ${H_DIR}


.c.o:
	gcc ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	make -C ${LFT}
	gcc ${FLAGS} ${LIBFLAGS} ${OBJS} -o ${NAME} -g -fsanitize=address

all: ${NAME}

clean:
	make clean -C ${LFT}
	rm -f ${OBJS}

fclean: clean
	make fclean -C ${LFT}
	rm -f ${NAME}

re: fclean ${NAME}

bonus:
	${NAME}

.PHONY: all, clean, fclean, re, bonus