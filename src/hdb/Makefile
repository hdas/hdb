######################################################################
#
# Makefile for db2.1 for RedHat Linux 7.2
# by Hansaraj Das <das_hansaraj@hotmail.com>
#
######################################################################

SRCDIR = ./

INCLUDE_OPT = -I.

CFLAGS+=$(INCLUDE_OPT)

OBJS = aggr.o expr.o select.o where.o token.o db.o sql.o main.o hdbio.o hdbm.o dbclnt.o

OBJS1 = aggr.o expr.o select.o where.o token.o db.o sql.o main.o hdbio.o
OBJS2 = db.o token.o hdbm.o dbclnt.o

LDFLAGS=
LIBS=

all: hdb


hdb: $(OBJS)
	$(CC) -o hdb $(OBJS1) $(LDFLAGS) $(LIBS) -lpthread -lm
	$(CC) -o hdbm $(OBJS2) $(LDFLAGS) $(LIBS) -lpthread -lm
	rm $(OBJS)
	cp hdb /usr/bin/hdb
	cp hdbm /usr/bin/hdbm



clean:
	rm -f hdb $(OBJS)
	rm -f hdbm $(OBJS)


