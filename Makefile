NAME = Webserver

CXXFLAGS = -Wall -Wextra -Wshadow -std=c++98 
CXXFLAGS_EXTRA = -Wall -Wextra -Wshadow -std=c++98 -Wuninitialized -Wconversion -fsanitize=address,undefined

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

all: $(NAME)

extra: 
	c++ $(CXXFLAGS_EXTRA) $(SRCS) -o $(NAME)_extra

run: all 
	@./$(NAME)

$(NAME) : $(OBJS)
	c++ -Wall -Wextra -Wshadow -std=c++98 -o $(NAME) $(OBJS)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(NAME)_extra

re: fclean all
