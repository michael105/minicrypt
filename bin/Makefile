.PHONY: README.md blake3sums.txt

README.md: blake3sums.txt
	cp README.in README.md
	#ls -lho --time-style=long-iso | grep -oE '\s*(\S*\s*){5}$$' >> README.md
	#echo '```'>> README.md
	perl Makeindex.pl >> README.md
	echo '```'>> README.md
	echo -e "\n-----------\n blake3sum\n\n" >> README.md
	sh blake3sums.sh >> README.md



blake3sums.txt:
	sh blake3sums.sh > blake3sums.txt
