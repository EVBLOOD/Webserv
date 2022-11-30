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
		New/tcpListener.cpp
		

all: $(NAME)

$(NAME) : $(OBJS)
	c++ -Wall -Wextra -Wshadow -std=c++98 -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm $(NAME)

re: fclean all
