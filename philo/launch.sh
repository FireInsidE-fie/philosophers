make && valgrind --leak-check=full --track-origins=yes ./philosophers 5 1000 200 200 \
&& valgrind --tool=helgrind ./philosophers 5 1000 200 200