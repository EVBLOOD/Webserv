NAME = Webserver

CXXFLAGS = -Wall -Wextra -Wshadow -std=c++98

CXX = c++

OBJS	=	$(SRCS:%.cpp=%.o)

SRCS =  main.cpp\
        Request.cpp\
        tools.cpp\
		Response.cpp\
		New/TcpStream.cpp\
		New/kqueue.cpp\
		New/tcpListener.cpp\
		parsing/location.cpp\
		parsing/parser.cpp\
		parsing/serverInfo.cpp\
		parsing/tokengen.cpp\

all: $(NAME)

run: all 
	@./$(NAME)

$(NAME) : $(OBJS)
	c++ -Wall -Wextra -Wshadow -std=c++98 -o $(NAME) $(OBJS)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
