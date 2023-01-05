#include "email_sender.h"

struct upload_status {
  size_t bytes_read;
};

char *payload_text;

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;
  size_t room = size * nmemb;
 
  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }
 
  data = &payload_text[upload_ctx->bytes_read];
 
  if(data) {
    size_t len = strlen(data);
    if(room < len)
      len = room;
    memcpy(ptr, data, len);
    upload_ctx->bytes_read += len;
 
    return len;
  }
 
  return 0;
}

int send_email(char *message, char *sender, struct recipient* recipients, char *username, char *password, char *smtp_ip_adress, char *smtp_port)
{
  curl_global_init(CURL_GLOBAL_DEFAULT);
  int rc = 0;
  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipient_list = NULL;
  struct upload_status upload_ctx = {0};
  struct recipient* current_recipient = recipients;

  payload_text = malloc(sizeof(char) * strlen(message));
  strcpy(payload_text, message);
  curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "smtps");
    curl_easy_setopt(curl, CURLOPT_URL, smtp_ip_adress);
    curl_easy_setopt(curl, CURLOPT_PORT, atoi(smtp_port));
    curl_easy_setopt(curl, CURLOPT_USERNAME, username);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sender);

    while(current_recipient != NULL)
    {
      recipient_list = curl_slist_append(recipient_list, current_recipient->email);
      current_recipient = current_recipient->next;
    }
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipient_list);
    
    // rc = (int)curl_easy_perform(curl);
    curl_slist_free_all(recipient_list);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  free(payload_text);

  return rc;
}
