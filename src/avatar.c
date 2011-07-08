#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include <glib.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

static void *myrealloc(void *ptr, size_t size);

static void *myrealloc(void *ptr, size_t size)
{
  /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
}

static size_t
WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;

  mem->memory = myrealloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory) {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  }
  return realsize;
}

char* get_avatar(char* uin)
{
  CURL *curl_handle;
  char* xml_url=malloc(strlen(uin)+200*sizeof(char));
  struct MemoryStruct chunk;
  xmlnode xml = NULL;
  xmlnode xmlnode_users;
  xmlnode xmlnode_user;
  xmlnode xmlnode_avatars;
  xmlnode xmlnode_avatar;
  xmlnode xmlnode_bigavatar;
  char* is_blank;

  chunk.memory=NULL; /* we expect realloc(NULL, size) to work */
  chunk.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  sprintf(xml_url,"http://api.gadu-gadu.pl/avatars/%s/0.xml", uin);
  curl_easy_setopt(curl_handle, CURLOPT_URL, xml_url);

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 5.5)");

  /* get it! */
  curl_easy_perform(curl_handle);

  xml = xmlnode_str(chunk.memory, strlen(chunk.memory));
  xmlnode_users = xmlnode_get_tag(xml, "users");
  xmlnode_user = xmlnode_get_tag(xmlnode_users, "user");
  xmlnode_avatars = xmlnode_get_tag(xmlnode_user, "avatars");
  xmlnode_avatar = xmlnode_get_tag(xmlnode_avatars, "avatar");
  xmlnode_bigavatar = xmlnode_get_tag(xmlnode_avatar, "originBigAvatar");
  is_blank = xmlnode_get_attrib(xmlnode_avatar, "blank");
  xml_url = xmlnode_get_data(xmlnode_bigavatar);

  chunk.memory = NULL;
  chunk.size=0;

  curl_easy_setopt(curl_handle, CURLOPT_URL, xml_url);
  curl_easy_perform(curl_handle);

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  free(chunk.memory);

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();

  if(!strlen(is_blank))
   return g_base64_encode(chunk.memory,chunk.size);
  else
   return "0";
}
