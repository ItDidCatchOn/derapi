#include "json_print.h"

void json_print(json_object *jobj)
{
  const char* msg=json_object_get_string(jobj);
  int indent=0;
  char c=msg[0];
  bool inquotes=false;

  while(c!=0) {

    if(c=='"') inquotes=!inquotes;

    if(c!=' ' || inquotes) {

      if(c=='{' || c=='[') {
	indent+=2;
	printf("%c\n%.*s", c, indent, "                                                          ");

      } else if(c=='}' || c==']') {
	indent-=2;
	printf("\n%.*s%c", indent, "                                                          ", c);

      } else if(c==',') {
	printf(",\n%.*s", indent, "                                                          ");

      } else if(c==':') {
	printf(": ");

      } else printf("%c", c);
    }
    c=*(++msg);
  } 
  printf("\n");
}
