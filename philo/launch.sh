make
valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --fair-sched=yes ./philosophers 5 1000 200 20 7
valgrind --tool=helgrind --fair-sched=yes ./philosophers 5 1000 200 200 7
