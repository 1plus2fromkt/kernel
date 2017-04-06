cd build
if [[ "$1" = "B" ]]; then
	make -B
else 
	make
fi
