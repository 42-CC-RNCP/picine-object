NAME    = car
CC      = clang++
CFLAGS  = -Wall -Wextra -Werror -std=c++98 -g

SRCS    = main.cpp          # only .cpp files here
HEADERS = car.hpp           # optional, for completeness

OBJS    = $(SRCS:.cpp=.o)
RM      = rm -f

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)  # list headers only as *prerequisites*
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.cpp $(HEADERS)        # rebuild .o when headers change
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
