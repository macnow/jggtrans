#include "jabber.h"
#include "register.h"
#include "iq.h"
#include "users.h"
#include "sessions.h"
#include <glib.h>

const char *instructions;

void jabber_iq_get_register(Stream *s,const char *from,const char *id,xmlnode q){
xmlnode node;
xmlnode iq;
xmlnode query;
xmlnode instr;

	node=xmlnode_get_firstchild(q);
	if (node){
		g_warning("Get for jabber:iq:register not empty!: %s",xmlnode2str(q));
		jabber_iq_send_error(s,from,id,"query not empty");
		return;
	}		
	iq=xmlnode_new_tag("iq");
	xmlnode_put_attrib(iq,"type","result");
	if (id) xmlnode_put_attrib(iq,"id",id);
	xmlnode_put_attrib(iq,"to",from);
	xmlnode_put_attrib(iq,"from",my_name);
	query=xmlnode_insert_tag(iq,"query");
	xmlnode_put_attrib(query,"xmlns","jabber:iq:register");
	xmlnode_insert_tag(query,"username");
	xmlnode_insert_tag(query,"password");
	xmlnode_insert_tag(query,"name");
	xmlnode_insert_tag(query,"email");
	instr=xmlnode_insert_tag(query,"instructions");
	xmlnode_insert_cdata(instr,instructions,-1);
	stream_write(s,iq);
	xmlnode_free(iq);
}

void jabber_iq_set_register(Stream *s,const char *from,const char *id,xmlnode q){
xmlnode node;
char *username,*password;
uin_t uin;
User *user;
Session *session;
	
	node=xmlnode_get_firstchild(q);
	if (!node){
		g_warning("Set query for jabber:iq:register empty: %s",xmlnode2str(q));
		jabber_iq_send_error(s,from,id,"query is empty");
		return;
	}		
	node=xmlnode_get_tag(q,"username");
	if (node) username=xmlnode_get_data(node);
	if (!node || !username){
		g_warning("Username not given: %s",xmlnode2str(q));
		jabber_iq_send_error(s,from,id,"username not given");
		return;
	}		
	uin=atoi(username);
	node=xmlnode_get_tag(q,"password");
	if (node) password=xmlnode_get_data(node);
	if (!node || !password){
		g_warning("Password not given: %s",xmlnode2str(q));
		jabber_iq_send_error(s,from,id,"password not given");
		return;
	}		

	user=user_create(from,uin,password);
	if (!user){
		g_warning("Couldn't create user %s",from);
		jabber_iq_send_error(s,from,id,"Registration failed");
		return;
	}
	
	session=session_create(user,from,id,q,s);
	if (!user){
		g_warning("Couldn't create session for %s",from);
		jabber_iq_send_error(s,from,id,"Registration failed");
		return;
	}	
}
