/* $Id: users.h,v 1.14 2003/01/14 14:25:25 jajcus Exp $ */

/*
 *  (C) Copyright 2002 Jacek Konieczny <jajcus@pld.org.pl>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License Version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef users_h
#define users_h

#include <libgadu.h>

typedef struct contact_s{
	uin_t uin;
	char * first;
	char * last;
	char * nick;
	char * display;
	char * phone;
	char * group;
	char * email;
	char * x1,*x2,*x3;
	
	int status;
	GTime last_update;
	char *status_desc;
	uint32_t ip;
	uint16_t port;
	uint32_t version;
}Contact;

typedef struct user_s{
	uin_t uin;
	char * jid;
	char * password;
	int last_sys_msg;
	int friends_only;
	int invisible;

	int confirmed;
	GList *contacts;
}User;

extern GHashTable *users_jid;

User *user_create(const char *jid,uin_t uin,const char * password);
int user_remove(User *u);
int user_delete(User *u);

int user_save(User *u);

User *user_get_by_jid(const char *jid);

int user_subscribe(User *u,uin_t uin);
int user_unsubscribe(User *u,uin_t uin);

int user_sys_msg_received(User *u,int nr);

int user_set_contact_status(User *u,int status,unsigned int uin,char *desc,
				int more,uint32_t ip,uint16_t port,uint32_t version);

void user_print(User *u,int indent);
int users_probe_all();
int users_init();
int users_done();

#endif
