#include "message_database.h"

sqlite3* DATABASE = NULL;
const char* LOG_PATH = "/var/log/mqtt_messages.db";

void get_current_time(char** time_string)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(*time_string,"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void create_database()
{
    char* err=NULL;

    char* table = "CREATE TABLE IF NOT EXISTS mqtt_messages ("
                "topic TEXT,"
                "payload TEXT,"
                "time TEXT);";
    
    int rc = sqlite3_exec(DATABASE, table, NULL, NULL, &err);
    if(rc != SQLITE_OK){
        printf("Failed to create database\n");
    }
    sqlite3_free(err);
}

void open_database_file()
{
    int rc = sqlite3_open(LOG_PATH, &DATABASE); 
    if(rc!=SQLITE_OK){
        printf("Failed to open database\n");
        return;
    }
}

void close_database_file()
{
    int rc = sqlite3_close(DATABASE); 
    if(rc!=SQLITE_OK){
        printf("Failed to close database\n");
        return;
    }
}

void write_to_database(char* topic, char* payload)
{
    char* err;
    char* time_string=malloc(sizeof(char)*255);
    get_current_time(&time_string);

    char* temp = "INSERT INTO mqtt_messages (topic, payload, time) VALUES";
    char dbtext[255];
    // todo:
    // remove literal sql injection
    snprintf(dbtext, 255, "%s('%s', '%s', '%s');",temp, topic, payload, time_string);
    int rc = sqlite3_exec(DATABASE, dbtext, NULL, NULL, &err);
    if(rc != SQLITE_OK){
        printf("Failed to insert log.\n");
    }
    free(time_string);
}

sqlite3** get_database_pointer()
{
    return &DATABASE;
}