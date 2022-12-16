NAME = Webserver

CXXFLAGS = -Wall -Wextra -Wshadow -std=c++98
CXXFLAGS_EXTRA = -Wall -Wextra -Wshadow -std=c++98 -Wuninitialized -fsanitize=address,undefined

CXX = c++

OBJS	=	$(SRCS:%.cpp=%.o)

SRCS =  main.cpp\
        Request.cpp\
        tools.cpp\
		Response.cpp\
		socket/TcpStream.cpp\
		socket/kqueue.cpp\
		socket/tcpListener.cpp\
		parsing/location.cpp\
		parsing/parser.cpp\
		parsing/serverInfo.cpp\
		parsing/tokengen.cpp\


SRCS_TEST =  tests.cpp\
			Request.cpp\
			tools.cpp\
			Response.cpp\
			socket/TcpStream.cpp\
			socket/kqueue.cpp\
			socket/tcpListener.cpp\
			parsing/location.cpp\
			parsing/parser.cpp\
			parsing/serverInfo.cpp\
			parsing/tokengen.cpp\


all: $(NAME)

extra: 
	c++ $(CXXFLAGS_EXTRA) $(SRCS) -o $(NAME)_extra &&./$(NAME)_extra 

test: $(TEST)
	c++ $(CXXFLAGS) $(SRCS_TEST) -o $(NAME)_test
	./$(NAME)_test 


fast: $(NAME) $(SRCS)
	c++ $(CXXFLAGS) $(SRCS) -O3 -DFAST -o $(NAME)_fast
	./$(NAME)_fast 


run: fclean all 
	@./$(NAME)

$(NAME) : $(OBJS)
	c++ -Wall -Wextra -Wshadow -std=c++98 -o $(NAME) $(OBJS)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(NAME)_extra
	@rm -rf $(NAME)_test

re: fclean all
