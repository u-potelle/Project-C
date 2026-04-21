gcc -I include main.c src/*.c -o build/app -lreadline

exec ./build/app
