ALL: cmp exc

cmp: main.c bitfile.c
	gcc -o huffman main.c bitfile.c

exc:
	@./huffman

git:
	git add .
	git commit -m "$m"
	git push -u origin main