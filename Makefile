NAME = Webserver

CXXFLAGS = -Wall -Wextra -Werror -std=c++98
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
		socket/File.cpp\
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


HEADERS = socket/kqueue.hpp\
			socket/tcpListener.hpp\
			socket/TcpStream.hpp\
			Request.hpp\
			Response.hpp\
			tools.hpp

all: $(NAME)

extra: $(SRCS) $(HEADERS)
	c++ $(CXXFLAGS_EXTRA) $(SRCS) -o $(NAME)_extra && ./$(NAME)_extra config

test: $(TEST) $(HEADERS)
	c++ $(CXXFLAGS) $(SRCS_TEST) -o $(NAME)_test
	./$(NAME)_test


run:  all 
	@./$(NAME) config

$(NAME) : $(HEADERS) $(SRCS) $(OBJS) 
	c++ -Wall -Wextra -Wshadow -std=c++98 -o $(NAME) $(OBJS)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(NAME)_extra
	@rm -rf $(NAME)_test

re: fclean all
