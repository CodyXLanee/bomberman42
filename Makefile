.PHONY: all clean fclean re install

NAME = ft_vox

CC = clang++
SRC =	src/Sdl_gl_win.cpp \
		main.cpp \


CFLAGS = -Wall -Wextra -Werror
OBJ_PATH = ./obj/
OBJ_NAME = $(SRC:.cpp=.o)
PACKAGES = sdl2 freetype2 glm

PATHS = $(shell pkg-config --cflags $(PACKAGES))
LIBS = $(shell pkg-config --libs $(PACKAGES))

OPENGL = -framework OpenGl -framework AppKit

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

all: $(NAME)
$(OBJ_PATH)%.o: %.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/libs 2> /dev/null || echo "" > /dev/null
	$(CC) $(CFLAGS) -o $@ -c -Isrc $(PATHS) $< -std=c++11

$(NAME): $(OBJ)
	$(CC) $(SDL) $(LIBS) $(OPENGL) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

install:
	./script.sh
	make