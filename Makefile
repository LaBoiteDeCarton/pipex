NAME = pipex
S_DIR = srcs
H_DIR = includes
SRCS_FILES = main.c
SRCS = $(addprefix ${S_DIR}/, ${SRCS_FILES})
OBJS = ${SRCS:.c=.o}
FLAGS = -Wall -Wextra -Werror -I ${H_DIR}


.c.o:
	gcc ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	gcc ${FLAGS} ${OBJS} -o ${NAME}

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean ${NAME}

bonus:
	${NAME}

.PHONY: all, clean, fclean, re, bonus