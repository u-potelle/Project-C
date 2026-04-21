gcc -I include main.c src/*.c -o build/app -lreadline
cd build
exec ./app
cd ../
