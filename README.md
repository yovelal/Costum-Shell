# Costum-Shell
Linux "shell" program in c

//////The author contact information//////
    Yovel Aloni 

//////Description//////
    A shell program that supports the following command:
    - pwd
    - cd
    - nano
    - cat >
    - cat
    - wc or wc + flags
    - cp
    - pipe
    - clear
    - grep or grep + flags
    - ls or ls + flags
    - tree
    - exit

//////How to run the program//////
    Make sure the installations are done:
    sudo apt-get install tree
    sudo apt-get install libreadline-dev

    To run, enter the following command :
    gcc main.c main -lreadline;./main 
