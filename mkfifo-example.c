#include <strings.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static char buf[8192];

void creator(const char *path) {
  mode_t mode = 0600;
  if (mkfifo(path, mode) != 0) {
    perror("Can't create fifo");
    exit(1);
  }
}

void reader(const char *path) {
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    perror("Could not open FIFO");
    exit(1);
  }

  while (1) {
    ssize_t numbytes = read(fd, &buf, sizeof(buf));
    
    if (numbytes == 0) exit(0);
    
    if (numbytes == -1) {
      perror("Could not read FIFO");
      exit(1);
    }

    buf[numbytes] = '\0';
    puts(buf);
  }
}

void writer(const char *path) {
  int fd = open(path, O_WRONLY);
  if (fd == -1) {
    perror("Could not open FIFO");
    exit(1);
  }

  char msg1[] = "Hello darkness my old friend";
  if (write(fd, msg1, sizeof(msg1)) == -1) goto fail;
  sleep(2);

  char msg2[] = "I've come to talk to you again";
  if (write(fd, msg2, sizeof(msg2)) == -1) goto fail;
  sleep(2);

  char msg3[] = "Because a vision softly creeping";
  if (write(fd, msg3, sizeof(msg3)) == -1) goto fail;
  sleep(2);

  char msg4[] = "Left its seeds while I was sleeping";
  if (write(fd, msg4, sizeof(msg4)) == -1) goto fail;

  return;

 fail:
  perror("Could not write to FIFO");
  exit(1);  
}
    
    

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s creator|reader|writer fifoname\n", argv[0]);
    exit(1);
  }

  const char *fn = argv[1];
  const char *path = argv[2];
  
  if (strcmp(fn, "creator") == 0) {
    creator(path);
  } else if (strcmp(fn, "reader") == 0) {
    reader(path);
  } else if (strcmp(fn, "writer") == 0) {
    writer(path);
  } else {
    printf("Invalid method %s", fn);
    exit(1);
  }
  
  return 0;
}
