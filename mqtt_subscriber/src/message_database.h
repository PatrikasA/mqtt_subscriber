#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <syslog.h>


void create_database();
void open_database_file();
void close_database_file();
void write_to_database(char* topic, char* payload);
sqlite3** get_database_pointer();