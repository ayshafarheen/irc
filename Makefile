# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juhaamid <juhaamid@student.42abudhabi.a    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: Invalid date        by                   #+#    #+#              #
#    Updated: 2024/05/06 12:44:46 by juhaamid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME = ircserv

MY_SOURCES = main.cpp server.cpp client.cpp parse.cpp

MY_OBJS = $(MY_SOURCES:.cpp=.o)

CXXFLAGS =  -Wall -Wextra -Werror 

CXX = c++

all: $(NAME)

$(NAME): $(MY_OBJS)
	$(CXX) $(CXXFLAGS) $(MY_SOURCES) -o $(NAME)

clean:
	rm -f $(MY_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
