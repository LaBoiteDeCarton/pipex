NAME = pipex
S_DIR = srcs
H_DIR = includes
SRCS_FILES = main.c \
				heredoc.c \
				utils.c
SRCS = $(addprefix ${S_DIR}/, ${SRCS_FILES})
OBJS = ${SRCS:.c=.o}
LFT = lib/libft
LIBFLAGS = -Llib/libft -lft
FLAGS = -I ${H_DIR} #-Wall -Wextra -Werror


.c.o:
	gcc ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	make -C ${LFT}
	gcc ${FLAGS} ${LIBFLAGS} ${OBJS} -o ${NAME}

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