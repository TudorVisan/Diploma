all:
	arm-linux-gnueabi-gcc read.c -o read_arm -lpthread
	gcc read.c -o read_x86 -lpthread
	gcc ftp_file_upload.c -o ftp_file_upload
ftp:
	./ftp_file_upload
run:
	sudo ./read_x86
clean:
	rm read_arm read_x86

upload:
	nc 192.168.1.1 60000 < read_arm
