NAME=btc
CPP=c++
CPPFLAGS=-Wall -Wextra -Werror -std=c++98


SRCS=main.cpp BitcoinExchange.cpp

OBJS=$(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp BitcoinExchange.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f  $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re