NAME =  PmergeMe
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp PmergeMe.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lstdc++

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
